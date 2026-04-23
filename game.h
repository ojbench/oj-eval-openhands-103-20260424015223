#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <typeinfo>

struct Node;
struct GameState;
class Card;
class Pile;

/**
 * Skill card type
 * MULTIPLIER: Increases the score multiplier for the next score card
 * HEAD_BUTT: Moves the card at the bottom (tail) of the discard pile to the top (head) of the draw pile
 */
enum SkillCardType {
    MULTIPLIER,
    HEAD_BUTT,
};

/** 
 * Card base class
 * TODO: Add member variables and functions if needed
 */
class Card {
public:
    explicit Card() = default;
    virtual ~Card() = default;
    // Execute card effect, implemented by derived classes. You need to implement the specific effect of the current card type in each derived class
    virtual void play(GameState& game_state) = 0;
};

/** 
 * Score card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class ScoreCard: public Card {
private:
    int point;
    
public:
    explicit ScoreCard(int point);
    void play(GameState &game_state) override;
    ~ScoreCard() override = default;
};

/** 
 * Skill card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class SkillCard: public Card {
private:
    SkillCardType skill_type;
    
public:
    explicit SkillCard(SkillCardType skill_card_type);
    void play(GameState &game_state) override;
    ~SkillCard() override = default;
};

/**
 * Power card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class PowerCard: public Card {
private:
    int plus_count;
    
public:
    explicit PowerCard(int plus_count);
    void play(GameState &game_state) override;
    ~PowerCard() override = default;
};

/** 
 * Linked list node structure
 * TODO: Add or modify member variables and functions as needed
 */
struct Node {
    Card* card;
    Node* next;
    Node* prev;
    
    Node(Card* c) : card(c), next(nullptr), prev(nullptr) {}
};

/** 
 * Pile class
 * All three piles (draw pile, discard pile, hand pile) are represented using this class
 * TODO: Modify as needed, add member variables and functions
 */
class Pile {
private:
    int size_;
    Node* head;
    Node* tail;

public:
    friend void outShuffle(GameState&);
    friend void inShuffle(GameState&);
    friend void oddEvenShuffle(GameState&);
    friend class GameController;
    friend class SkillCard;

    // Constructor and destructor declarations
    // Please implement them in the later part of the file
    Pile();
    ~Pile();

    // Return pile size
    int size() const {
        return size_;
    }

    // Check if pile is empty
    bool empty() const {
        return size_ == 0;
    }

    // Add a card to the end of the pile
    // This function is used to initialize the pile in main.cpp
    // Please implement this function in the later part of the file
    void appendCard(Card* card);
    
    // Remove and return the first card (from head)
    Card* removeHead();
    
    // Remove and return the card at position index (1-based from head)
    Card* removeAt(int index);
    
    // Add a card to the end of the pile (tail)
    void addToTail(Card* card);
    
    // Get head and tail nodes for friend functions
    Node* getHead() const { return head; }
    Node* getTail() const { return tail; }
    
    // Set head and tail nodes for friend functions
    void setHead(Node* new_head) { head = new_head; }
    void setTail(Node* new_tail) { tail = new_tail; }
    
    // Clear the pile
    void clear();
};

/**
 * Game state structure
 * Stores current game state information such as discard pile, draw pile, hand pile status, total score, current multiplier, etc.
 * TODO: Add member variables and functions as needed
 */
struct GameState {
    Pile hand_{};            // Hand pile
    Pile draw_pile_{};       // Draw pile
    Pile discard_pile_{};    // Discard pile
    int total_score{0};      // Total score
    int current_multiplier{1}; // Current multiplier
    int accumulated_bonus{0}; // Accumulated power bonus
};

/** 
 * Shuffle function declarations
 * outShuffle: Shuffle the discard pile into the draw pile in order
 * inShuffle: Shuffle the discard pile into the draw pile in reverse order
 * oddEvenShuffle: Separate the discard pile by odd and even positions, with odd positions first and even positions last
 * Please implement these functions in the later part of this file
*/
void outShuffle(GameState &game_state);
void inShuffle(GameState &game_state);
void oddEvenShuffle(GameState &game_state);

/**
 * Game controller class
 * Used to handle game logic
 */
class GameController {
private:
    GameState game_state_;
    // Shuffle function pointer
    void (*shuffle_)(GameState&);
public:
    explicit GameController(int mode);

    // Draw 5 cards from the draw pile to the hand pile
    // This is a function declaration, please implement this function in the later part of the file
    void draw();

    // Play the card_to_play-th card in the hand pile (counting from 1)
    // This is a function declaration, please implement this function in the later part of the file
    void play(int card_to_play);

    // End the current turn, move all cards in the hand pile to the discard pile
    // This is a function declaration, please implement this function in the later part of the file
    void finish();

    // Shuffle in the specified manner
    // This is a function declaration, please implement this function in the later part of the file
    void shuffle();

    // Query current total score
    // This is a function declaration, please implement this function in the later part of the file
    int queryScore();

    // Query draw pile size
    int queryDrawPileSize() const {
        return game_state_.draw_pile_.size();
    }

    // Query hand pile size
    int queryHandSize() const {
        return game_state_.hand_.size();
    }

    // Query discard pile size
    int queryDiscardPileSize() const {
        return game_state_.discard_pile_.size();
    }

    // Get reference to the draw pile
    // Used to load cards in main.cpp
    Pile& drawPile() {
        return game_state_.draw_pile_;
    }
};

// ================= 洗牌函数实现 ===================

/**
 * Shuffle the discard pile into the draw pile in order
 * TODO: Implement this function
 */
void outShuffle(GameState& game_state) {
    if (game_state.discard_pile_.empty()) return;
    
    // Append discard pile to draw pile in order
    if (game_state.draw_pile_.empty()) {
        // Draw pile is empty, just move discard pile to draw pile
        game_state.draw_pile_.setHead(game_state.discard_pile_.getHead());
        game_state.draw_pile_.setTail(game_state.discard_pile_.getTail());
    } else {
        // Connect tail of draw pile to head of discard pile
        game_state.draw_pile_.getTail()->next = game_state.discard_pile_.getHead();
        game_state.discard_pile_.getHead()->prev = game_state.draw_pile_.getTail();
        game_state.draw_pile_.setTail(game_state.discard_pile_.getTail());
    }
    
    // Update draw pile size
    game_state.draw_pile_.size_ += game_state.discard_pile_.size_;
    
    // Clear discard pile
    game_state.discard_pile_.setHead(nullptr);
    game_state.discard_pile_.setTail(nullptr);
    game_state.discard_pile_.size_ = 0;
}

/**
 * Shuffle the discard pile into the draw pile in reverse order
 * TODO: Implement this function
 */
void inShuffle(GameState& game_state) {
    if (game_state.discard_pile_.empty()) return;
    
    // Create a new list in reverse order
    Node* new_head = nullptr;
    Node* new_tail = nullptr;
    int new_size = 0;
    
    Node* current = game_state.discard_pile_.getTail();
    while (current != nullptr) {
        Node* new_node = new Node(current->card);
        
        if (new_head == nullptr) {
            new_head = new_node;
            new_tail = new_node;
        } else {
            new_tail->next = new_node;
            new_node->prev = new_tail;
            new_tail = new_node;
        }
        new_size++;
        
        current = current->prev;
    }
    
    // Append reversed list to draw pile
    if (game_state.draw_pile_.empty()) {
        game_state.draw_pile_.setHead(new_head);
        game_state.draw_pile_.setTail(new_tail);
    } else {
        game_state.draw_pile_.getTail()->next = new_head;
        new_head->prev = game_state.draw_pile_.getTail();
        game_state.draw_pile_.setTail(new_tail);
    }
    
    game_state.draw_pile_.size_ += new_size;
    
    // Clear discard pile (but don't delete cards since they're moved)
    game_state.discard_pile_.setHead(nullptr);
    game_state.discard_pile_.setTail(nullptr);
    game_state.discard_pile_.size_ = 0;
}

/**
 * Separate the discard pile by odd and even positions, with odd positions first and even positions last, and shuffle into the draw pile
 * TODO: Implement this function
 */
void oddEvenShuffle(GameState& game_state) {
    if (game_state.discard_pile_.empty()) return;
    
    // Separate into odd and even positions
    std::vector<Card*> odd_cards;
    std::vector<Card*> even_cards;
    
    Node* current = game_state.discard_pile_.getHead();
    int position = 1;
    
    while (current != nullptr) {
        if (position % 2 == 1) {
            odd_cards.push_back(current->card);
        } else {
            even_cards.push_back(current->card);
        }
        current = current->next;
        position++;
    }
    
    // Create new list: odd positions first, then even positions
    Node* new_head = nullptr;
    Node* new_tail = nullptr;
    int new_size = 0;
    
    // Add odd cards
    for (Card* card : odd_cards) {
        Node* new_node = new Node(card);
        
        if (new_head == nullptr) {
            new_head = new_node;
            new_tail = new_node;
        } else {
            new_tail->next = new_node;
            new_node->prev = new_tail;
            new_tail = new_node;
        }
        new_size++;
    }
    
    // Add even cards
    for (Card* card : even_cards) {
        Node* new_node = new Node(card);
        
        if (new_head == nullptr) {
            new_head = new_node;
            new_tail = new_node;
        } else {
            new_tail->next = new_node;
            new_node->prev = new_tail;
            new_tail = new_node;
        }
        new_size++;
    }
    
    // Append new list to draw pile
    if (game_state.draw_pile_.empty()) {
        game_state.draw_pile_.setHead(new_head);
        game_state.draw_pile_.setTail(new_tail);
    } else {
        game_state.draw_pile_.getTail()->next = new_head;
        new_head->prev = game_state.draw_pile_.getTail();
        game_state.draw_pile_.setTail(new_tail);
    }
    
    game_state.draw_pile_.size_ += new_size;
    
    // Clear discard pile (but don't delete cards since they're moved)
    game_state.discard_pile_.setHead(nullptr);
    game_state.discard_pile_.setTail(nullptr);
    game_state.discard_pile_.size_ = 0;
}
    
// ======================================================




// ================== Game State Structure Implementation ===================
// If you added functions to the GameState structure, you can implement them here





// ======================================================




// ================= Card Class Implementation ===========================

// === Score Card Class Implementation ===
ScoreCard::ScoreCard(int point) {
    this->point = point;
}

void ScoreCard::play(GameState &game_state) {
   // Calculate score: (card point + accumulated bonus) * current multiplier
   int score = (point + game_state.accumulated_bonus) * game_state.current_multiplier;
   game_state.total_score += score;
   
   // Reset multiplier after using score card
   game_state.current_multiplier = 1;
}
// ===================

// === Skill Card Class Implementation ===
SkillCard::SkillCard(SkillCardType skill_card_type) {
    this->skill_type = skill_card_type;
}

void SkillCard::play(GameState &game_state) {
    if (skill_type == MULTIPLIER) {
        // Increase multiplier by 1
        game_state.current_multiplier++;
    } else if (skill_type == HEAD_BUTT) {
        // Move tail of discard pile to head of draw pile
        if (!game_state.discard_pile_.empty()) {
            Card* card = game_state.discard_pile_.removeAt(game_state.discard_pile_.size());
            if (card != nullptr) {
                // Add to head of draw pile
                Node* newNode = new Node(card);
                Node* currentHead = game_state.draw_pile_.getHead();
                
                if (currentHead == nullptr) {
                    game_state.draw_pile_.setHead(newNode);
                    game_state.draw_pile_.setTail(newNode);
                } else {
                    newNode->next = currentHead;
                    currentHead->prev = newNode;
                    game_state.draw_pile_.setHead(newNode);
                }
                
                // Increase draw pile size
                game_state.draw_pile_.size_++;
            }
        }
    }
}
// ===================

// === Power Card Class Implementation ===
PowerCard::PowerCard(int plus_count) {
    this->plus_count = plus_count;
}

void PowerCard::play(GameState &game_state) {
    // Add to accumulated bonus (permanent effect)
    game_state.accumulated_bonus += plus_count;
}
// ===================

// ======================================================




// ================= Pile Class Implementation ===========================
Pile::Pile() {
    head = nullptr;
    tail = nullptr;
    size_ = 0;
}

Pile::~Pile() {
    clear();
}

/**
  * Add a card to the end of the pile
  * TODO: Implement this function
  */
void Pile::appendCard(Card* card) {
    addToTail(card);
}

Card* Pile::removeHead() {
    if (head == nullptr) return nullptr;
    
    Node* temp = head;
    Card* card = temp->card;
    
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    
    delete temp;
    size_--;
    return card;
}

Card* Pile::removeAt(int index) {
    if (index < 1 || index > size_) return nullptr;
    
    Node* current = head;
    for (int i = 1; i < index; i++) {
        current = current->next;
    }
    
    Card* card = current->card;
    
    if (current == head && current == tail) {
        head = nullptr;
        tail = nullptr;
    } else if (current == head) {
        head = head->next;
        head->prev = nullptr;
    } else if (current == tail) {
        tail = tail->prev;
        tail->next = nullptr;
    } else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    
    delete current;
    size_--;
    return card;
}

void Pile::addToTail(Card* card) {
    Node* newNode = new Node(card);
    
    if (tail == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    
    size_++;
}

void Pile::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current->card;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    size_ = 0;
}

// ======================================================




// ================= Game Controller Class Implementation ======================

/**
 * Constructor, initialize game state and shuffle mode
 * @param mode Shuffle mode, 1 for outShuffle, 2 for inShuffle, 3 for oddEvenShuffle
 * TODO: Implement this function
 */
GameController::GameController(int mode){
    // Initialize shuffle function based on mode
    switch (mode) {
        case 1:
            shuffle_ = outShuffle;
            break;
        case 2:
            shuffle_ = inShuffle;
            break;
        case 3:
            shuffle_ = oddEvenShuffle;
            break;
        default:
            shuffle_ = outShuffle; // Default to outShuffle
            break;
    }
}
/**
 * Draw 5 cards from the draw pile to the hand pile
 * TODO: Implement this function
 */
void GameController::draw() {
    // Try to draw 5 cards
    for (int i = 0; i < 5; i++) {
        // If draw pile is empty, try to shuffle
        if (game_state_.draw_pile_.empty()) {
            shuffle();
        }
        
        // If still empty after shuffle, can't draw more
        if (game_state_.draw_pile_.empty()) {
            break;
        }
        
        // Draw one card from draw pile to hand pile
        Card* card = game_state_.draw_pile_.removeHead();
        if (card != nullptr) {
            game_state_.hand_.addToTail(card);
        }
    }
}

/**
 * Play the card_to_play-th card in the hand pile (counting from 1)
 * TODO: Implement this function
 * Reference implementation approach:
    * 1. Find the corresponding hand card node
    * 2. Remove the card from the hand pile
    * 3. Execute the card's effect
    * 4. If it's a score card or skill card, move it to the discard pile; if it's a power card, delete it
 */
void GameController::play(int card_to_play) {
    // Remove the card from hand pile
    Card* card = game_state_.hand_.removeAt(card_to_play);
    if (card == nullptr) return;
    
    // Execute the card's effect
    card->play(game_state_);
    
    // Check card type and handle accordingly
    ScoreCard* score_card = dynamic_cast<ScoreCard*>(card);
    SkillCard* skill_card = dynamic_cast<SkillCard*>(card);
    PowerCard* power_card = dynamic_cast<PowerCard*>(card);
    
    if (score_card != nullptr || skill_card != nullptr) {
        // Move to discard pile
        game_state_.discard_pile_.addToTail(card);
    } else if (power_card != nullptr) {
        // Power cards are destroyed after use
        delete card;
    }
}

/**
 * Shuffle in the specified manner
 * TODO: Implement this function
 */
void GameController::shuffle() {
    // Call the shuffle function
    shuffle_(game_state_);
}

/**
 * End the current turn, move all cards in the hand pile to the discard pile
 * TODO: Implement this function
 */
void GameController::finish() {
    // Move all cards from hand pile to discard pile
    while (!game_state_.hand_.empty()) {
        Card* card = game_state_.hand_.removeHead();
        if (card != nullptr) {
            game_state_.discard_pile_.addToTail(card);
        }
    }
}

/**
 * Query current total score
 * TODO: Implement this function
 */
int GameController::queryScore() {
    return game_state_.total_score;
}

#endif //GAME_H
