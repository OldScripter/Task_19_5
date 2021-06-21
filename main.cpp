#include <iostream>
#include <vector>
#include <fstream>

const int QUESTIONS_COUNT = 13;

/*
 * Read files into vector. Return status of error into 'errorFlag' (true if error).
 */
void readAssets(std::vector<std::string>& v, const std::string& assetName, bool& errorFlag)
{
    std::string path;
    for (int i = 1; i <= QUESTIONS_COUNT; i++)
    {
        path = "..\\" + assetName + "_" + std::to_string(i) + ".txt";
        std::ifstream stream (path);
        if (stream.is_open())
        {
            std::string content;
            content = "";
            while (!stream.eof())
            {
                char* buffer = new char[100];
                stream.read(buffer,100);
                if (stream.gcount() < 100) buffer[stream.gcount()] = '\0';
                content += buffer;
            }
            v.push_back(content);
            stream.close();
        }
        else
        {
            std::cerr << "\t - File read error:" << path << "\n";
            errorFlag = true;
        }
    }
    std::cout << assetName <<"s are successfully read.\n";
}

/*
 * Returns the index of next non-played question laying in 'offset' units after 'currentQuestionIndex'.
 * Takes into account non-played questions vector<bool>.
 * Important! Uses QUESTIONS_COUNT global.
 */
int findNextQuestion(const std::vector<bool>& nonPlayedQuestions, const int currentQuestionIndex, const int offset)
{
    int result = (currentQuestionIndex + offset) % QUESTIONS_COUNT;
    for (int i = 0; i < QUESTIONS_COUNT; i++)
    {
        int index = (result + i) % QUESTIONS_COUNT;
        if (nonPlayedQuestions[index]) return index;
    }
    return -1;
}

/*
 * Returns the offset entered from key board and true / false to 'brakeFlag' if input is 'q'.
 */
int getOffset(bool& brakeFlag)
{
    bool incorrectInput;
    int offset = 0;
    do
    {
        incorrectInput = false;
        std::string input;
        std::cout << "Please enter the offset (greater than 0) or 'q' to exit:\n";
        std::getline(std::cin, input);
        if (input == "q")
        {
            brakeFlag = true;
            return 0;
        }
        else
        {
            brakeFlag = false;
            try
            {
                offset = std::stoi (input);
                if (offset < 0)
                {
                    std::cerr << "Invalid offset (less than 0), please try again.\n";
                    incorrectInput = true;
                }
            }
            catch (const std::invalid_argument& ia)
            {
                incorrectInput = true;
                std::cerr << "Invalid offset, please try again.\n";
            }
        }
    }
    while (incorrectInput);

    return  offset;
}

/*
 * Set all questions status to 'non-played'. Fills the vector with 'true' values.
 */
void resetQuestionStatus(std::vector<bool>& questionsStatus)
{
    for (int i = 0; i < QUESTIONS_COUNT; i++)
    {
        questionsStatus.push_back(true);
    }
}

/*
 * Prints the winner or report that game is stopped if no winners is revealed.
 */
void getWinner(const int scoresPlayer_1, const int scoresPlayer_2, const int maxScores)
{
    if (scoresPlayer_1 == maxScores || scoresPlayer_2 == maxScores)
    {
        std::cout << "The winner is " << (scoresPlayer_1 == maxScores ? "Player 1" : "Player 2") << "\n";
        std::cout << "Congratulations!\n";
    }
    else
    {
        std::cout << "Game is stopped\n";
    }
}


int main() {
    bool isReadingError = false;
    std::vector<std::string> questions;
    std::vector<std::string> answers;
    std::vector<bool> nonPlayedQuestions;

    //Reading questions:
    readAssets(questions, "question", isReadingError);
    if (isReadingError) return 0;

    //Reading answers:
    readAssets(answers, "answer", isReadingError);
    if (isReadingError) return 0;

    //Reset the questions status to 'non-answered'
    resetQuestionStatus(nonPlayedQuestions);

    std::cout << "-----------------------\n";
    std::cout << "Game is started!\n";
    int scoresPlayer_1 = 0;
    int scoresPlayer_2 = 0;
    int currentQuestionIndex = 0;
    bool isStopped = false;
    while (scoresPlayer_1 < 7 && scoresPlayer_2 < 7)
    {
        //take offset
        int offset = getOffset(isStopped);
        if (isStopped) break;
        std::cout << "You've entered " << offset << "\n";

        //find question
        int nextQuestionIndex = findNextQuestion(nonPlayedQuestions, offset, currentQuestionIndex);
        if (nextQuestionIndex == -1) break;
        std::cout << "Question " << (nextQuestionIndex + 1) << " is chosen.\n";
        nonPlayedQuestions[nextQuestionIndex] = false;

        //ask question
        std::cout << "Question:\n" << questions[nextQuestionIndex] << "\n";

        //take answer
        std::cout << "Your answer: ";
        std::string answer;
        std::getline(std::cin, answer);

        //check answer
        if (answer == answers[nextQuestionIndex])
        {
            scoresPlayer_1++;
            std::cout << "Correct!\n";
        }
        else
        {
            scoresPlayer_2++;
            std::cout << "Wrong! The correct answer is " << answers[nextQuestionIndex] << "\n";
        }

        //print scores
        std::cout << "Player 1 scores: " << scoresPlayer_1 << "\n";
        std::cout << "Player 2 scores: " << scoresPlayer_2 << "\n";


        //check if next turn is necessary
        if (scoresPlayer_1 != 7 && scoresPlayer_2 != 7)
        {
            std::cout << "--------------Next turn!--------------\n";
            currentQuestionIndex = nextQuestionIndex;
        }
        else
            std::cout << "-----------Game is finished-----------\n";
    }
    getWinner(scoresPlayer_1, scoresPlayer_2, 7);

    return 0;
}
