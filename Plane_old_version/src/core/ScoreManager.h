/**
 * @file ScoreManager.h
 * @brief Definition of the ScoreManager class representing a score manager object.
 */

#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <iostream>
#include <fstream>

/**
 * @class ScoreManager
 * @brief A class representing a score manager object.
 */
class ScoreManager {
private:
    int playerScore;        /**< The current player score. */
    int highScore;          /**< The current high score. */
    int totalScore;         /**< The total score. */

public:
    // Constructors and destructors
    /**
     * @brief Construct a new ScoreManager object.
     */
    ScoreManager();

    /**
     * @brief Destroy the ScoreManager object.
     */
    ~ScoreManager();

    // Getter and setter functions
    /**
     * @brief Get the current player score.
     * @return The current player score.
     */
    int getPlayerScore() const;

    /**
     * @brief Set the current player score.
     * @param score The new player score.
     */
    void setPlayerScore(int score);

    /**
     * @brief Get the current high score.
     * @return The current high score.
     */
    int getHighScore() const;

    /**
     * @brief Set the current high score.
     * @param score The new high score.
     */
    void setHighScore(int score);

    /**
     * @brief Get the total score.
     * @return The total score.
     */
    int getTotalScore();

    // Loading and saving score functions
    /**
     * @brief Load the score from a file.
     */
    void loadScore();

    /**
     * @brief Save the score to a file.
     */
    void saveScore();

    // Updating high score function
    /**
     * @brief Update the high score.
     * @param score The new score to compare with the current high score.
     */
    void updateHighScore(int score);

    // Recording game score and time function
    /**
     * @brief Record the game score and time.
     * @param score The game score.
     * @param gameTime The game time.
     */
    void recordGame(int score, float gameTime);   
};

#endif // SCOREMANAGER_H
