//
// Created by Marvens on 4/19/2025.
//

#ifndef PROJECT_3_DSA_APPLICATIONWINDOW_H
#define PROJECT_3_DSA_APPLICATIONWINDOW_H

#endif //PROJECT_3_DSA_APPLICATIONWINDOW_H

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
using namespace sf;
using namespace std;


// ApplicationWindow class and methods
class ApplicationWindow{
    // Data structure
    map<string, Texture> cardTextures;

public:
    /* Constructor
    ApplicationWindow(Font font){
        Texture buffer;
        /* buffer.loadFromFile("./Files/logo.png")
        cardTextures.emplace("logo", buffer);
        logo.setTexture(gametextures["logo"]);
        buffer.loadFromFile("./Files/search_button.png")
        cardTextures.emplace("search_button", buffer);
        search_button.setTexture(gametextures["search_button"]);
    } */

    void setText(Text &text, float x, float y){
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(Vector2f(x, y));
    }

    void createApplicationWindow(){
        const unsigned int window_width = 1000;
        const unsigned int window_height = 900;

        // Creating the main window
        RenderWindow window(VideoMode(window_width, window_height), "Opti-Cart");
        window.setFramerateLimit(60);

        // Loading font
        Font font;
        if (!font.loadFromFile("./cmake-build-debug/Files/LSANS.TTF")){
            cout << "Font loading failure" << endl;
            return;
        }

        // Card Display
        RectangleShape cardDisplay(Vector2f(509, 700));
        cardDisplay.setPosition((window_width - 509.0) / 2, 100);
        cardDisplay.setOutlineColor(Color::Black);
        cardDisplay.setOutlineThickness(3);
        cardDisplay.setFillColor(Color::White);

        // Card Results
        RectangleShape cardResults(Vector2f(300, 50));
        cardResults.setPosition((window_width - 300.0) / 2, 820);
        cardResults.setOutlineColor(Color::Black);
        cardResults.setOutlineThickness(2);
        cardResults.setFillColor(Color::White);

        // Search Box
        RectangleShape searchBox(Vector2f(200, 40));
        searchBox.setPosition(40, 50);
        searchBox.setOutlineColor(Color::Black);
        searchBox.setOutlineThickness(2);
        searchBox.setFillColor(Color::White);

        // Search Button
        RectangleShape search_button(Vector2f(30, 30));
        search_button.setPosition(250, 55);
        search_button.setOutlineColor(Color::Black);
        search_button.setOutlineThickness(1);
        search_button.setFillColor(Color(230, 230, 230));

        // Search Results
        RectangleShape searchResults(Vector2f(200, 300));
        searchResults.setPosition(40, 100);
        searchResults.setOutlineColor(Color::Black);
        searchResults.setOutlineThickness(2);
        searchResults.setFillColor(Color::White);

        // Vendor Box
        RectangleShape vendorBox(Vector2f(200, 40));
        vendorBox.setPosition(window_width - 240, 50);
        vendorBox.setOutlineColor(Color::Black);
        vendorBox.setOutlineThickness(2);
        vendorBox.setFillColor(Color::White);

        // Vendor Results
        RectangleShape vendorResults(Vector2f(200, 300));
        vendorResults.setPosition(window_width - 240, 100);
        vendorResults.setOutlineColor(Color::Black);
        vendorResults.setOutlineThickness(2);
        vendorResults.setFillColor(Color::White);

        // Search text (reusing your variable)
        Text initialSearch("Type in a card.", font, 16);
        initialSearch.setFillColor(Color::Black);
        initialSearch.setPosition(50, 55);

        // Vendors text
        Text vendorsText("Vendors", font, 18);
        vendorsText.setFillColor(Color::Black);
        vendorsText.setPosition(window_width - 230, 55);

        // Card placeholder text
        Text cardPlaceholder("Card Image", font, 30);
        cardPlaceholder.setFillColor(Color::Black);
        cardPlaceholder.setPosition((window_width - 509) / 2 + 175, 300);

        // Variables for search functionality
        string newCardName = "";
        float scrollOffset = 0.0f;
    }

    void updateSearchResults(RenderWindow& window, RectangleShape &searchResults, Font &font, string &newCardName, Event &currentEvent, float& scrollOffset){
        const float card_display_height = 40.0f;
        const float card_padding = 5.0f;
        const float container_padding = 10.0f;
        const float cancel_button_size = 24.0f;

        // Make the cards vector static to persist between calls
        static vector<string> savedCards;

        // Process new card additions
        if (!newCardName.empty()){
            // Check for duplicates
            bool duplicatesExist = false;
            for (const auto& card : savedCards){
                if (card == newCardName){
                    duplicatesExist = true;
                    break;
                }
            }
            if (!duplicatesExist){
                savedCards.push_back(newCardName);
            }
            // Clear after processing
            newCardName.clear();
        }

        // Calculate max scroll offset
        float contentHeight = savedCards.size() * (card_display_height + card_padding);
        float maxScroll = max(0.0f, contentHeight -(searchResults.getSize().y - 2 * container_padding));

        // Handle mouse wheel scrolling
        if (currentEvent.type == Event::MouseWheelScrolled && currentEvent.mouseWheelScroll.wheel == Mouse::VerticalWheel){
            Vector2i mousePos = Mouse::getPosition(window);
            if (searchResults.getGlobalBounds().contains(Vector2f(mousePos))){
                // Scroll calculation
                scrollOffset -= currentEvent.mouseWheelScroll.delta * 20.f;
                // Constrain scroll limits
                scrollOffset = max(0.0f, min(scrollOffset, maxScroll));
            }
        }

        // Handle remove card button clicks
        if (currentEvent.type == Event::MouseButtonPressed &&
            currentEvent.mouseButton.button == Mouse::Left){

            Vector2i mousePos(currentEvent.mouseButton.x, currentEvent.mouseButton.y);

            if (searchResults.getGlobalBounds().contains(Vector2f(mousePos))){
                float yPos = searchResults.getPosition().y + container_padding - scrollOffset;

                for (size_t i = 0; i < savedCards.size();){
                    FloatRect cardRect(
                            searchResults.getPosition().x + container_padding,yPos,searchResults.getSize().x - 2 * container_padding,card_display_height);

                    FloatRect removeRect(cardRect.left + cardRect.width - cancel_button_size - card_padding,cardRect.top + (card_display_height - cancel_button_size) / 2.0f,cancel_button_size,cancel_button_size);

                    if (removeRect.contains(Vector2f(mousePos))){
                        savedCards.erase(savedCards.begin() + i);
                    } else{
                        yPos += card_display_height + card_padding;
                        i++;
                    }
                }
            }
        }

        // Draw the container
        window.draw(searchResults);

        // Set up view for scrolling and clipping
        View originalView = window.getView();
        View resultsView = originalView;

        FloatRect clipRect(
                searchResults.getPosition().x + 2,
                searchResults.getPosition().y + 2,
                searchResults.getSize().x - 4,
                searchResults.getSize().y - 4
        );

        resultsView.reset(FloatRect(
                clipRect.left,
                clipRect.top + scrollOffset,
                clipRect.width,
                clipRect.height
        ));

        window.setView(resultsView);

        // Draw all cards
        float yPos = searchResults.getPosition().y + container_padding;

        for (const auto& cardName : savedCards){
            // Card background
            RectangleShape cardBG(Vector2f(
                    searchResults.getSize().x - 2 * container_padding,
                    card_display_height
            ));
            cardBG.setPosition(
                    searchResults.getPosition().x + container_padding,
                    yPos
            );
            cardBG.setFillColor(Color(240, 240, 240));
            cardBG.setOutlineColor(Color(200, 200, 200));
            cardBG.setOutlineThickness(1);

            // Card name text
            Text nameText(cardName, font, 16);
            nameText.setFillColor(Color::Black);
            nameText.setPosition(
                    cardBG.getPosition().x + 10.0f,
                    cardBG.getPosition().y + (card_display_height - nameText.getLocalBounds().height) / 2.0f - 5.0f
            );

            // Cancel button
            RectangleShape cancelButton(Vector2f(cancel_button_size, cancel_button_size));
            cancelButton.setPosition(
                    cardBG.getPosition().x + cardBG.getSize().x - cancel_button_size - card_padding,
                    cardBG.getPosition().y + (card_display_height - cancel_button_size) / 2.0f
            );
            cancelButton.setFillColor(Color(220, 220, 220));
            cancelButton.setOutlineColor(Color(180, 180, 180));
            cancelButton.setOutlineThickness(1);

            // X text for cancel button
            Text xText("X", font, 14);
            xText.setFillColor(Color::Black);
            xText.setPosition(
                    cancelButton.getPosition().x + (cancel_button_size - xText.getLocalBounds().width) / 2.0f - 2.0f,
                    cancelButton.getPosition().y + (cancel_button_size - xText.getLocalBounds().height) / 2.0f - 5.0f
            );

            // Draw all elements
            window.draw(cardBG);
            window.draw(nameText);
            window.draw(cancelButton);
            window.draw(xText);

            // Move to next position
            yPos += card_display_height + card_padding;
        }

        // Restore original view
        window.setView(originalView);
    }

    string searchBoxInput(const Event &event, RectangleShape &searchBox, const Font &font){
        // Tracking the input state
        enum InputState{entering_card_name,entering_quantity};
        // Variables for managing input state (static variables are for variables persisting between function calls)
        static InputState currentState = entering_card_name;
        string cardNameInput;
        string quantityInput;
        static bool searchBoxActive = false;
        Clock blinkClock;
        bool showCursor = true;
        string result = "";

        // Updating cursor blink
        if (blinkClock.getElapsedTime().asSeconds() > 0.5f){
            showCursor = !showCursor;
            blinkClock.restart();
        }

        string &currentInput = (currentState == entering_card_name) ? cardNameInput : quantityInput;

        // Event handling
        if (event.type == Event::MouseButtonPressed){
            Vector2i mousePos = Mouse::getPosition();
            searchBoxActive = searchBox.getGlobalBounds().contains(Vector2f(mousePos));
            if (searchBoxActive){
                showCursor = true;
                blinkClock.restart();
            }
        }
        else if (event.type == Event::TextEntered && searchBoxActive){
            // Handling user input
            if (event.text.unicode == 8){ // Backspace
                if (!currentInput.empty()){
                    currentInput.pop_back();
                }
            }
            else if (event.text.unicode == 13){ // Enter
                if (currentState == entering_card_name && !currentInput.empty()){
                    currentState = entering_quantity;
                    quantityInput.clear();
                }
                else if (currentState == entering_quantity && !currentInput.empty()){
                    result = cardNameInput + " (x" + quantityInput + ")";
                }
                cardNameInput.clear();
                quantityInput.clear();
                currentState = entering_card_name;
            }
            showCursor = true;
            blinkClock.restart();
        }
        else if (event.text.unicode < 128){
            // Allow only numbers for quantity, and only printable characters for card names
            if (currentState == entering_quantity){
                if (event.text.unicode >= '0' && event.text.unicode <= '9'){
                    currentInput += static_cast<char>(event.text.unicode);
                }
            }
            else{
                if (event.text.unicode >= 32){
                    currentInput += static_cast<char>(event.text.unicode);
                }
            }
        }
        else if (event.type == Event::KeyPressed){
            // Escape key cancels input
            if (event.key.code == Keyboard::Escape && searchBoxActive){
                currentInput.clear();
                searchBoxActive = false;
            }
        }
        return result;
    }

    void drawSearchInput(RenderWindow& window, RectangleShape& searchBox, Font& font){
        enum InputState{entering_card_name, entering_quantity};
        static InputState currentState = entering_card_name;
        static string cardNameInput;
        static string quantityInput;
        static bool searchBoxActive = false;
        static Clock blinkClock;
        static bool showCursor = true;

        // Update cursor blink
        if (blinkClock.getElapsedTime().asSeconds() > 0.5f){
            showCursor = !showCursor;
            blinkClock.restart();
        }

        string& currentInput = (currentState == entering_card_name) ? cardNameInput : quantityInput;

        // Create display string with cursor
         string displayString = currentInput;
        if (searchBoxActive && showCursor){
            displayString += "|";
        }

        // Draw input text or placeholder
        if (currentInput.empty() && (!searchBoxActive || !showCursor)){
            Text placeholderText;
            placeholderText.setFont(font);
            placeholderText.setCharacterSize(16);
            placeholderText.setFillColor(Color(150, 150, 150));
            placeholderText.setPosition(searchBox.getPosition().x + 10,
                                        searchBox.getPosition().y + (searchBox.getSize().y - 24) / 2);

            if (currentState == entering_card_name){
                placeholderText.setString("Type in a card.");
            } else{
                placeholderText.setString("Enter quantity:");
            }

            window.draw(placeholderText);
        } else{
            Text inputText;
            inputText.setFont(font);
            inputText.setString(displayString);
            inputText.setCharacterSize(16);
            inputText.setFillColor(Color::Black);
            inputText.setPosition(searchBox.getPosition().x + 10,searchBox.getPosition().y + (searchBox.getSize().y - 24) / 2);
            window.draw(inputText);
        }
    }

    void updateScreen(RenderWindow& window, Font& font, RectangleShape& cardDisplay, RectangleShape& cardResults, RectangleShape& searchBox, RectangleShape& search_button, RectangleShape& searchResults, RectangleShape& vendorBox, RectangleShape& vendorResults, Event& currentEvent, string& newCardName, float& scrollOffset){
        // Clear window before drawing
        window.clear(Color::White);

        // Draw base UI elements
        window.draw(cardDisplay);
        window.draw(cardResults);
        window.draw(searchBox);
        window.draw(search_button);
        window.draw(searchResults);
        window.draw(vendorBox);
        window.draw(vendorResults);

        // Draw card placeholder
        Text cardPlaceholder("Card Image", font, 30);
        cardPlaceholder.setFillColor(Color::Black);
        cardPlaceholder.setPosition(cardDisplay.getPosition().x + cardDisplay.getSize().x/2 - 80,
                                    cardDisplay.getPosition().y + cardDisplay.getSize().y/2 - 15);
        window.draw(cardPlaceholder);

        // Draw vendor title
        Text vendorsText("Vendors", font, 18);
        vendorsText.setFillColor(Color::Black);
        vendorsText.setPosition(vendorBox.getPosition().x + 60, vendorBox.getPosition().y + 8);
        window.draw(vendorsText);

        // Draw search button text
        Text searchButtonText("S", font, 16);
        searchButtonText.setFillColor(Color::Black);
        searchButtonText.setPosition(search_button.getPosition().x + 7, search_button.getPosition().y + 5);
        window.draw(searchButtonText);

        // Draw card result information
        Text resultsInfoText("There are XX results for [CARD]", font, 16);
        resultsInfoText.setFillColor(Color::Black);
        resultsInfoText.setPosition(cardResults.getPosition().x + 15, cardResults.getPosition().y + 15);
        window.draw(resultsInfoText);

        // Draw search box input state
        drawSearchInput(window, searchBox, font);

        // Update and draw search results
        updateSearchResults(window, searchResults, font, newCardName, currentEvent, scrollOffset);

        // Update the display
        window.display();
    }

};