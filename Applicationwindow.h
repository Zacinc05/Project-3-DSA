//
// Created by Marvens on 4/19/2025.
//

#ifndef PROJECT_3_DSA_APPLICATIONWINDOW_H
#define PROJECT_3_DSA_APPLICATIONWINDOW_H

#endif //PROJECT_3_DSA_APPLICATIONWINDOW_H

#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
using namespace sf;
using namespace std;


// ApplicationWindow class and methods
class ApplicationWindow{
    // Data structure
    map<string, Texture> cardTextures;
    // SFML Objects
    RenderWindow window;
    Font font;

    // UI components
    RectangleShape cardDisplay;
    RectangleShape cardResults;
    RectangleShape searchBox;
    RectangleShape search_button;
    RectangleShape searchResults;
    RectangleShape vendorBox;
    RectangleShape vendorResults;

    // Variables
    string newCardName;
    string currentCardToDisplay;
    float scrollOffset;
    // State variables (static)
    enum InputState{entering_card_name, entering_quantity};
    static InputState currentInputState;
    static string cardNameInputString;
    static string quantityInputString;
    static bool isSearchBoxActive;
    static Clock cursorBlinkClock;
    static bool showCursorState;

public:
    // Constructor
    ApplicationWindow() : scrollOffset(0.0f){}

    void initializeApplicationWindow(){
        const unsigned int window_width = 1200;
        const unsigned int window_height = 900;
        const float padding = 20.0f;

        window.create(VideoMode(window_width, window_height), "Opti-Cart");
        window.setFramerateLimit(60);

        if (!this->font.loadFromFile("./Files/LSANS.TTF"))
            cout << "Font loading failure. Check path." << endl;


        // Rectangle sizes
        Vector2f searchBoxSize(200, 40);
        Vector2f searchButtonSize(40, 40);
        Vector2f vendorBoxSize(300, 40);
        Vector2f cardDisplaySize(480, 670);
        Vector2f searchResultsSize(220, cardDisplaySize.y);
        Vector2f vendorResultsSize(vendorBoxSize.x, cardDisplaySize.y);
        Vector2f cardResultsCountSize(cardDisplaySize.x, 100);

        // UI calculations
        float topRowY = padding;
        Vector2f searchBoxPos(padding, topRowY);
        Vector2f searchButtonPos(searchBoxPos.x + searchBoxSize.x + 10, topRowY);
        Vector2f vendorBoxPos(window_width - padding - vendorBoxSize.x, topRowY);
        float mainContentY = topRowY + searchBoxSize.y + padding;
        Vector2f searchResultsPos(padding, mainContentY);
        Vector2f vendorResultsPos(vendorBoxPos.x, mainContentY);
        float spaceBetweenColumns = vendorResultsPos.x - (searchResultsPos.x + searchResultsSize.x);
        float cardDisplayX = searchResultsPos.x + searchResultsSize.x + (spaceBetweenColumns - cardDisplaySize.x) / 2.0f;
        Vector2f cardDisplayPos(cardDisplayX, mainContentY);
        float bottomRowY = mainContentY + cardDisplaySize.y + padding;
        Vector2f cardResultsCountPos(cardDisplayPos.x, bottomRowY);

        searchBox.setSize(searchBoxSize);
        searchBox.setPosition(searchBoxPos);
        searchBox.setOutlineColor(Color::Black);
        searchBox.setOutlineThickness(3);
        searchBox.setFillColor(Color::White);

        search_button.setSize(searchButtonSize);
        search_button.setPosition(searchButtonPos);
        search_button.setOutlineColor(Color::Black);
        search_button.setOutlineThickness(3);
        search_button.setFillColor(Color(230, 230, 230));

        vendorBox.setSize(vendorBoxSize);
        vendorBox.setPosition(vendorBoxPos);
        vendorBox.setOutlineColor(Color::Black);
        vendorBox.setOutlineThickness(3);
        vendorBox.setFillColor(Color::White);

        searchResults.setSize(searchResultsSize);
        searchResults.setPosition(searchResultsPos);
        searchResults.setOutlineColor(Color::Black);
        searchResults.setOutlineThickness(3);
        searchResults.setFillColor(Color(245, 245, 245));

        cardDisplay.setSize(cardDisplaySize);
        cardDisplay.setPosition(cardDisplayPos);
        cardDisplay.setOutlineColor(Color::Black);
        cardDisplay.setOutlineThickness(3);
        cardDisplay.setFillColor(Color::White);

        vendorResults.setSize(vendorResultsSize);
        vendorResults.setPosition(vendorResultsPos);
        vendorResults.setOutlineColor(Color::Black);
        vendorResults.setOutlineThickness(3);
        vendorResults.setFillColor(Color(245, 245, 245));

        cardResults.setSize(cardResultsCountSize);
        cardResults.setPosition(cardResultsCountPos);
        cardResults.setOutlineColor(Color::Black);
        cardResults.setOutlineThickness(3);
        cardResults.setFillColor(Color::White);

        newCardName = "";
        currentCardToDisplay = "";
        scrollOffset = 0.0f;
    }

    void updateSearchResults(Event &currentEvent){
        const float card_display_height = 40.0f;
        const float card_padding = 5.0f;
        const float container_padding = 10.0f;
        const float cancel_button_size = 24.0f;
        static vector<string> savedCards;

        if (!newCardName.empty()){
            // Check for duplicates before adding
            bool duplicatesExist = false;
            for (const auto& card : savedCards){
                string baseNewCardName = newCardName.substr(0, newCardName.find(" (x"));
                if (card.rfind(baseNewCardName, 0) == 0) {
                    duplicatesExist = true;
                    break;
                }
            }
            if (!duplicatesExist){
                savedCards.push_back(newCardName);
            }
            newCardName.clear();
        }

        // Calculate max scroll offset based on current content height
        float contentHeight = savedCards.size() * (card_display_height + card_padding);
        float viewHeight = searchResults.getSize().y - (2 * container_padding);
        float maxScroll = max(0.0f, contentHeight - viewHeight);

        // Handle mouse wheel scrolling only if the mouse is over the searchResults area
        if (currentEvent.type == Event::MouseWheelScrolled && currentEvent.mouseWheelScroll.wheel == Mouse::VerticalWheel){
            Vector2i mousePos = Mouse::getPosition(window);
            if (searchResults.getGlobalBounds().contains(static_cast<Vector2f>(mousePos))){
                scrollOffset -= currentEvent.mouseWheelScroll.delta * 20.f;
                // Constraining scroll limits
                scrollOffset = max(0.0f, min(scrollOffset, maxScroll));
            }
        }


        // Handle remove card button
        if (currentEvent.type == Event::MouseButtonPressed &&
            currentEvent.mouseButton.button == Mouse::Left){

            Vector2i mousePosPx = Vector2i(currentEvent.mouseButton.x, currentEvent.mouseButton.y);

            // Check if click is within the visual bounds of the search results container
            if (searchResults.getGlobalBounds().contains(static_cast<Vector2f>(mousePosPx))){
                Vector2f mousePosRelative = static_cast<Vector2f>(mousePosPx) - searchResults.getPosition();
                mousePosRelative.y += scrollOffset;
                float currentCardY = container_padding;

                for (size_t i = 0; i < savedCards.size();){
                    FloatRect cardRect(container_padding,currentCardY,searchResults.getSize().x - (2 * container_padding),card_display_height);
                    FloatRect removeRect(cardRect.left + cardRect.width - cancel_button_size - card_padding,cardRect.top + (card_display_height - cancel_button_size) / 2.0f,cancel_button_size,cancel_button_size);

                    // Check if the adjusted relative mouse position is inside this remove button's rectangle
                    if (removeRect.contains(mousePosRelative)){
                        savedCards.erase(savedCards.begin() + i);

                        // After removing, recalculate maxScroll
                        contentHeight = savedCards.size() * (card_display_height + card_padding);
                        viewHeight = searchResults.getSize().y - (2 * container_padding);
                        maxScroll = max(0.0f, contentHeight - viewHeight);
                        // Adjust scrollOffset if it's now past the new maxScroll
                        scrollOffset = min(scrollOffset, maxScroll);
                    } else{
                        currentCardY += card_display_height + card_padding;
                        i++;
                    }
                }
            }
        }

        // Setting up clipping view
        View originalView = window.getView(); // Store the original view

        // Create a view that matches the inner area of the searchResults box
        View resultsView(FloatRect(searchResults.getPosition().x + container_padding,searchResults.getPosition().y + container_padding - scrollOffset,searchResults.getSize().x - 2 * container_padding,searchResults.getSize().y - 2 * container_padding));

        // Set the viewport for the view to map it correctly onto the screen where the searchResults box is
        resultsView.setViewport(FloatRect(searchResults.getPosition().x / window.getSize().x,searchResults.getPosition().y / window.getSize().y,searchResults.getSize().x / window.getSize().x,searchResults.getSize().y / window.getSize().y));

        // Apply the custom view for drawing the list items
        window.setView(resultsView);

        // Draw all card entries within the clipping view
        float yDrawPos = searchResults.getPosition().y + container_padding;

        for (const auto& cardName : savedCards){
            // Card background shape
            RectangleShape cardBG(Vector2f(searchResults.getSize().x - 2 * container_padding,card_display_height));
            cardBG.setPosition(searchResults.getPosition().x + container_padding,yDrawPos);
            cardBG.setFillColor(Color(240, 240, 240));
            cardBG.setOutlineColor(Color(200, 200, 200));
            cardBG.setOutlineThickness(1);

            // Card name text
            Text nameText(cardName, font, 16); // Use member font
            nameText.setFillColor(Color::Black);
            // Position text relative to the card background
            FloatRect textBounds = nameText.getLocalBounds();
            nameText.setPosition(cardBG.getPosition().x + 10.0f,cardBG.getPosition().y + (card_display_height - textBounds.height) / 2.0f - nameText.getCharacterSize()*0.2f);

            // Cancel button
            RectangleShape cancelButton(Vector2f(cancel_button_size, cancel_button_size));
            cancelButton.setPosition(cardBG.getPosition().x + cardBG.getSize().x - cancel_button_size - card_padding,cardBG.getPosition().y + (card_display_height - cancel_button_size) / 2.0f);
            cancelButton.setFillColor(Color(220, 220, 220));
            cancelButton.setOutlineColor(Color(180, 180, 180));
            cancelButton.setOutlineThickness(1);

            // X text for the cancel button
            Text xText("X", font, 14); // Use member font
            xText.setFillColor(Color::Black);
            FloatRect xBounds = xText.getLocalBounds();
            xText.setPosition(cancelButton.getPosition().x + (cancel_button_size - xBounds.width) / 2.0f - xBounds.left,cancelButton.getPosition().y + (cancel_button_size - xBounds.height) / 2.0f - xBounds.top
            );

            // Draw all elements for this card entry (within the active view)
            window.draw(cardBG);
            window.draw(nameText);
            window.draw(cancelButton);
            window.draw(xText);

            // Move draw position down for the next card entry
            yDrawPos += card_display_height + card_padding;
        }

        // Restore original view
        window.setView(originalView);
    }

    string searchBoxInput(const Event &event){
        // Tracking the input state
        string result = "";

        // Check for mouse click to activate/deactivate search box
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){
            Vector2i mousePos = Mouse::getPosition(window); // Use member window
            // Check if click is inside the searchBox rectangle shape
            isSearchBoxActive = searchBox.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
            if(isSearchBoxActive) {
                showCursorState = true;
                cursorBlinkClock.restart();
            }
        }

        // Handle text entry only if the search box is active
        if (isSearchBoxActive && event.type == Event::TextEntered){
            char enteredChar = static_cast<char>(event.text.unicode);
            if (event.text.unicode == 8){ //Backspace
                if (currentInputState == entering_card_name && !cardNameInputString.empty()){
                    cardNameInputString.pop_back();
                } else if (currentInputState == entering_quantity && !quantityInputString.empty()){
                    quantityInputString.pop_back();
                } else if (currentInputState == entering_quantity && quantityInputString.empty()){
                    currentInputState = entering_card_name;
                }
            }
            else if (event.text.unicode == 13){ // Enter
                if (currentInputState == entering_card_name && !cardNameInputString.empty()){
                    currentInputState = static_cast<ApplicationWindow::InputState>(entering_quantity); // Move to quantity state
                    quantityInputString.clear();
                }
                else if (currentInputState == entering_quantity && !quantityInputString.empty()){
                    // Finalizing search string only when quantity is entered
                    result = cardNameInputString + " (x" + quantityInputString + ")";
                    // Clear for next search
                    cardNameInputString.clear();
                    quantityInputString.clear();
                    currentInputState = static_cast<ApplicationWindow::InputState>(entering_card_name);
                }
                else if (currentInputState == entering_quantity && quantityInputString.empty() && !cardNameInputString.empty()){
                    result = cardNameInputString;
                    cardNameInputString.clear();
                    currentInputState = static_cast<ApplicationWindow::InputState>(entering_card_name);
                }
            }
            else if (event.text.unicode < 128 && event.text.unicode >= 32){ // Normal chars
                if (currentInputState == entering_card_name){
                    cardNameInputString += enteredChar;
                } else if (currentInputState == entering_quantity){
                    // Allow only numerical digits for quantity
                    if (enteredChar >= '0' && enteredChar <= '9'){
                        quantityInputString += enteredChar;
                    }
                }
            }
            // Reset cursor blink timer on any valid input action
            showCursorState = true;
            cursorBlinkClock.restart();
        }
        else if (isSearchBoxActive && event.type == Event::KeyPressed && event.key.code == Keyboard::Escape){ // Escape key
            if(currentInputState == entering_quantity){
                quantityInputString.clear();
                currentInputState = entering_card_name;
            } else {
                cardNameInputString.clear();
                isSearchBoxActive = false;
            }
        }
        // Return the finalized search string
        return result;
    }

    void drawSearchInput(){
        // Update cursor blink state based on static timer
        if (cursorBlinkClock.getElapsedTime().asSeconds() > 0.5f){
            showCursorState = !showCursorState;
            cursorBlinkClock.restart();
        }
        // Determining inputState
        string& currentInputRef = (currentInputState == entering_card_name) ? cardNameInputString : quantityInputString;
        string displayString = currentInputRef;

        // Add blinking cursor character "|" if the box is active
        if (isSearchBoxActive && showCursorState){
            displayString += "|";
        }

        // Initializing text
        Text inputText;
        inputText.setFont(font);
        inputText.setCharacterSize(16);
        inputText.setFillColor(Color::Black);
        // Calculating center position for text
        float textPosY = searchBox.getPosition().y + (searchBox.getSize().y - inputText.getCharacterSize()) / 2.0f - 2;
        inputText.setPosition(searchBox.getPosition().x + 10, textPosY);

        // Show placeholder only if the box is inactive
        if (currentInputRef.empty() && !isSearchBoxActive){
            Text placeholderText;
            placeholderText.setFont(font);
            placeholderText.setCharacterSize(16);
            placeholderText.setFillColor(Color(150, 150, 150));
            placeholderText.setPosition(searchBox.getPosition().x + 10, textPosY);

            // Set placeholder text based on the current input state
            if (currentInputState == entering_card_name){
                placeholderText.setString("Enter a card name!");
            } else {
                placeholderText.setString("Enter a quantity!");
            }
            window.draw(placeholderText);
        } else {
            inputText.setString(displayString);
            window.draw(inputText);
        }
    }

    void updateCardDisplay(const string &cardNameToDisplay){
        // Static texture map used to cache any loaded textures
        static map<string, int> pokemonNumbers = {
                {"Bulbasaur", 1}, {"Ivysaur", 2}, {"Venusaur", 3}, {"Charmander", 4}, {"Charmeleon", 5}, {"Charizard", 6},
                {"Squirtle", 7}, {"Wartortle", 8}, {"Blastoise", 9}, {"Caterpie", 10}, {"Metapod", 11}, {"Butterfree", 12},
                {"Weedle", 13}, {"Kakuna", 14}, {"Beedrill", 15}, {"Pidgey", 16}, {"Pidgeotto", 17}, {"Pidgeot", 18},
                {"Rattata", 19}, {"Raticate", 20}, {"Spearow", 21}, {"Fearow", 22}, {"Ekans", 23}, {"Arbok", 24},
                {"Pikachu", 25}, {"Raichu", 26}, {"Sandshrew", 27}, {"Sandslash", 28}, {"Nidoran♀", 29}, {"Nidorina", 30},
                {"Nidoqueen", 31}, {"Nidoran♂", 32}, {"Nidorino", 33}, {"Nidoking", 34}, {"Clefairy", 35}, {"Clefable", 36},
                {"Vulpix", 37}, {"Ninetales", 38}, {"Jigglypuff", 39}, {"Wigglytuff", 40}, {"Zubat", 41}, {"Golbat", 42},
                {"Oddish", 43}, {"Gloom", 44}, {"Vileplume", 45}, {"Paras", 46}, {"Parasect", 47}, {"Venonat", 48},
                {"Venomoth", 49}, {"Diglett", 50}, {"Dugtrio", 51}, {"Meowth", 52}, {"Persian", 53}, {"Psyduck", 54},
                {"Golduck", 55}, {"Mankey", 56}, {"Primeape", 57}, {"Growlithe", 58}, {"Arcanine", 59}, {"Poliwag", 60},
                {"Poliwhirl", 61}, {"Poliwrath", 62}, {"Abra", 63}, {"Kadabra", 64}, {"Alakazam", 65}, {"Machop", 66},
                {"Machoke", 67}, {"Machamp", 68}, {"Bellsprout", 69}, {"Weepinbell", 70}, {"Victreebel", 71}, {"Tentacool", 72},
                {"Tentacruel", 73}, {"Geodude", 74}, {"Graveler", 75}, {"Golem", 76}, {"Ponyta", 77}, {"Rapidash", 78},
                {"Slowpoke", 79}, {"Slowbro", 80}, {"Magnemite", 81}, {"Magneton", 82}, {"Farfetch'd", 83}, {"Doduo", 84},
                {"Dodrio", 85}, {"Seel", 86}, {"Dewgong", 87}, {"Grimer", 88}, {"Muk", 89}, {"Shellder", 90},
                {"Cloyster", 91}, {"Gastly", 92}, {"Haunter", 93}, {"Gengar", 94}, {"Onix", 95}, {"Drowzee", 96},
                {"Hypno", 97}, {"Krabby", 98}, {"Kingler", 99}, {"Voltorb", 100}, {"Electrode", 101}, {"Exeggcute", 102},
                {"Exeggutor", 103}, {"Cubone", 104}, {"Marowak", 105}, {"Hitmonlee", 106}, {"Hitmonchan", 107}, {"Lickitung", 108},
                {"Koffing", 109}, {"Weezing", 110}, {"Rhyhorn", 111}, {"Rhydon", 112}, {"Chansey", 113}, {"Tangela", 114},
                {"Kangaskhan", 115}, {"Horsea", 116}, {"Seadra", 117}, {"Goldeen", 118}, {"Seaking", 119}, {"Staryu", 120},
                {"Starmie", 121}, {"Mr. Mime", 122}, {"Scyther", 123}, {"Jynx", 124}, {"Electabuzz", 125}, {"Magmar", 126},
                {"Pinsir", 127}, {"Tauros", 128}, {"Magikarp", 129}, {"Gyarados", 130}, {"Lapras", 131}, {"Ditto", 132},
                {"Eevee", 133}, {"Vaporeon", 134}, {"Jolteon", 135}, {"Flareon", 136}, {"Porygon", 137}, {"Omanyte", 138},
                {"Omastar", 139}, {"Kabuto", 140}, {"Kabutops", 141}, {"Aerodactyl", 142}, {"Snorlax", 143}, {"Articuno", 144},
                {"Zapdos", 145}, {"Moltres", 146}, {"Dratini", 147}, {"Dragonair", 148}, {"Dragonite", 149}, {"Mewtwo", 150}
        };

        if (cardNameToDisplay.empty()) {
            cardDisplay.setTexture(nullptr);
            cardDisplay.setFillColor(Color::White);
            return;
        }

        auto pokenumIter = pokemonNumbers.find(cardNameToDisplay);
        if (pokenumIter == pokemonNumbers.end()) {
            cout << "Pokemon card not found for: " << cardNameToDisplay << endl;
            cardDisplay.setTexture(nullptr);
            cardDisplay.setFillColor(Color::White);
            return;
        }
        int pokemonNumber = pokenumIter->second;
        string filePath = "./G1-Pokemon/" + to_string(pokemonNumber) + ".jpg";

        if (this->cardTextures.find(cardNameToDisplay) == this->cardTextures.end()) { // Check MEMBER cache
            Texture newTexture;
            if (!newTexture.loadFromFile(filePath)) {
                cout << "Failed to load texture file: " << filePath << endl;
                cardDisplay.setTexture(nullptr);
                cardDisplay.setFillColor(Color::White);
                return;
            }
            this->cardTextures[cardNameToDisplay] = newTexture;
        }
        try {
            cardDisplay.setTexture(&this->cardTextures.at(cardNameToDisplay), true);
            cardDisplay.setFillColor(Color::White);
        } catch (const std::out_of_range& oor) {
            cerr << "Error accessing texture cache for: " << cardNameToDisplay << endl;
            cardDisplay.setTexture(nullptr);
            cardDisplay.setFillColor(Color::White);
        }
    }

    void updateScreen(Event &currentEvent){
        // Clear window before drawing
        window.clear(Color::White);

        window.draw(cardResults);
        window.draw(searchBox);
        window.draw(search_button);
        window.draw(vendorBox);
        window.draw(vendorResults);

        Text vendorsTextLabel("Your Opti-Cart", font, 18);
        vendorsTextLabel.setFillColor(Color::Black);
        FloatRect vendorTextBounds = vendorsTextLabel.getLocalBounds();
        vendorsTextLabel.setPosition(vendorBox.getPosition().x + (vendorBox.getSize().x - vendorTextBounds.width) / 2.0f - vendorTextBounds.left,vendorBox.getPosition().y + (vendorBox.getSize().y - vendorTextBounds.height) / 2.0f - vendorTextBounds.top + 2);
        window.draw(vendorsTextLabel);

        Text searchButtonTextLabel("S", font, 20);
        searchButtonTextLabel.setFillColor(Color::Black);
        FloatRect searchBtnTxtBounds = searchButtonTextLabel.getLocalBounds();
        searchButtonTextLabel.setPosition(search_button.getPosition().x + (search_button.getSize().x - searchBtnTxtBounds.width) / 2.0f - searchBtnTxtBounds.left,search_button.getPosition().y + (search_button.getSize().y - searchBtnTxtBounds.height) / 2.0f - searchBtnTxtBounds.top);
        window.draw(searchButtonTextLabel);

        Text resultsInfoTextLabel("Algorithms PLACEHOLDER", font, 16);
        resultsInfoTextLabel.setFillColor(Color::Black);
        resultsInfoTextLabel.setPosition(cardResults.getPosition().x + 15,cardResults.getPosition().y + (cardResults.getSize().y - resultsInfoTextLabel.getCharacterSize()) / 2.0f);
        window.draw(resultsInfoTextLabel);

        drawSearchInput();
        updateSearchResults(currentEvent);
        updateCardDisplay(currentCardToDisplay);
        window.draw(cardDisplay);

        window.display();
    }

    void applicationRun(){
        Event event;
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                string finalizedSearch = searchBoxInput(event);
                if (!finalizedSearch.empty()) {
                    newCardName = finalizedSearch;

                    // Extract only the name for the display
                    string nameOnly = finalizedSearch;
                    size_t quantityPos = nameOnly.find(" (x");
                    if (quantityPos != string::npos){
                        nameOnly = nameOnly.substr(0, quantityPos);
                    }
                    nameOnly = nameOnly.substr(nameOnly.find_first_not_of(" \t"), nameOnly.find_last_not_of(" \t") + 1);
                    currentCardToDisplay = nameOnly;
                }
            }
            updateScreen(event);
        }
    }
};

ApplicationWindow::InputState ApplicationWindow::currentInputState = ApplicationWindow::entering_card_name;
string ApplicationWindow::cardNameInputString = "";
string ApplicationWindow::quantityInputString = "";
bool ApplicationWindow::isSearchBoxActive = false;
Clock ApplicationWindow::cursorBlinkClock;
bool ApplicationWindow::showCursorState = true;