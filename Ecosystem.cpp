#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Hare.h"
#include "Tile.h"
#include "HarePanel.h"
#include <sqlite3.h>
#include <ctime>

const int TILES_SIZE{5};

const int HARE_PANELS{ 5 };

const int TILES_GFX_SIZE{ 50 };
const int TILES_MARGIN{ 10 };

static int simulationID = 0;
static int haresID = 1;

int GetSimulationID() {
    return simulationID;
}
int GetNewHareID() {
    haresID++;
    return haresID;
}

void PrepareDatabase(sqlite3* db);

int main()
{
    sqlite3* db;
    //Otwieranie bazy danych
    if (sqlite3_open("example.db3", &db) != SQLITE_OK)
    {
        std::cerr << "Błąd przy otwieraniu bazy danych: " << sqlite3_errmsg(db) << std::endl;
    }
    PrepareDatabase(db);
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");

    std::cout << "Generate UI" << std::endl;
    sf::Font* mainFont = new sf::Font();
    if (!mainFont->loadFromFile("Bodoni.ttf"))
    {
        std::cerr << "Cant load font!" << std::endl;
        return -1;
    }

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("Background.jpg"))
    {
        std::cerr << "Cant load Background.jpg!" << std::endl;
        return -1;
    }

    sf::Sprite bg(bgTexture);
    float scaleX = (float)window.getSize().x / (float)bgTexture.getSize().x;
    float scaleY = (float)window.getSize().y / (float)bgTexture.getSize().y;
    bg.setScale(scaleX, scaleY);

    sf::Text dayCounterText;
    dayCounterText.setFont(*mainFont);
    dayCounterText.setCharacterSize(40);
    dayCounterText.setFillColor(sf::Color(161, 254, 161));
    dayCounterText.setStyle(sf::Text::Bold);
    dayCounterText.setPosition(460, 5);
    dayCounterText.setString("Simulation day: " + std::to_string(0));

    int startY = 75;
    int marginY = 5;
    int panelHeight = ((window.getSize().y - startY) / HARE_PANELS) - marginY;
    HarePanel* descPanels[HARE_PANELS];
    sf::Texture haresTexes[2][4] = {
        {sf::Texture(),sf::Texture(),sf::Texture(),sf::Texture()},
        {sf::Texture(),sf::Texture(),sf::Texture(),sf::Texture()}
    };
    for (int i = 0; i < 4; i++)
    {
        haresTexes[0][i].loadFromFile("Kicajec" + std::to_string(i) + ".png");
        haresTexes[1][i].loadFromFile("BabyKicajec" + std::to_string(i) + ".png");
    }
    for (int i = 0; i < HARE_PANELS; i++)
    {
        descPanels[i] = new HarePanel(450, (startY + ((100 + marginY) * i)), 300, panelHeight, mainFont, *haresTexes);
    }
    
    std::cout << "Generate terrain" << std::endl;

    Tile* tiles[TILES_SIZE][TILES_SIZE];

    for (int i = 0; i < TILES_SIZE; i++)
    {
        for (int j = 0; j < TILES_SIZE; j++)
        {
            tiles[i][j] = new Tile((rand() % 1500) + 1000, mainFont, TILES_GFX_SIZE);
        }
    }

    for (int i = 0; i < TILES_SIZE; i++)
    {
        for (int j = 0; j < TILES_SIZE; j++)
        {
            tiles[i][j]->SetPosition(((TILES_GFX_SIZE + TILES_MARGIN) * i), ((TILES_GFX_SIZE + TILES_MARGIN) * j));
            tiles[i][j]->SetTileNeighbors(
                tiles[(i == 0) ? (TILES_SIZE - 1) : (i - 1)][j],   // left
                tiles[i][(j == 0) ? (TILES_SIZE - 1) : (j - 1)],   // up
                tiles[(i == (TILES_SIZE - 1)) ? (0) : (i + 1)][j], // right
                tiles[i][(j == (TILES_SIZE - 1)) ? (0) : (j + 1)]  // down
            );

            std::cout << "Tile [" + std::to_string(i) + "][" + std::to_string(j) + "] <" + std::to_string((int)(tiles[i][j])) + ">" << std::endl;
        }
    }

    for (int i = 0; i < TILES_SIZE; i++)
    {
        for (int j = 0; j < TILES_SIZE; j++)
        {
            int howManyHares = rand() % 10;
            for (int z = 0; z < howManyHares; z++)
            {
                tiles[i][j]->AddHare(new Hare());
            }
        }
    }

    std::cout << "Starting simulation" << std::endl;

    bool runSimulation = true;
    int day{ 0 };
    Tile* selectedTile{ nullptr };
    int selectedOffset{ 0 };
    while (runSimulation)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                runSimulation = false;
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Space:
                    day++;
                    std::cout << "Day:" << std::to_string(day) << std::endl;
                    dayCounterText.setString("Simulation day:" + std::to_string(day));

                    for (int i = 0; i < TILES_SIZE; i++)
                    {
                        for (int j = 0; j < TILES_SIZE; j++)
                        {
                            tiles[i][j]->SimulateTile(db);
                        }
                    }

                    for (int i = 0; i < TILES_SIZE; i++)
                    {
                        for (int j = 0; j < TILES_SIZE; j++)
                        {
                            tiles[i][j]->SimulateMove();
                        }
                    }

                    break;
                }
                break;
            case sf::Event::MouseButtonPressed:
                switch (event.key.code)
                {
                case sf::Mouse::Left:
                    int xPos = sf::Mouse::getPosition(window).x;
                    int yPos = sf::Mouse::getPosition(window).y;

                    for (int i = 0; i < TILES_SIZE; i++)
                    {
                        for (int j = 0; j < TILES_SIZE; j++)
                        {
                            if (tiles[i][j]->IsClicked(xPos, yPos))
                            {
                                selectedTile = tiles[i][j];
                                selectedOffset = 0;
                            }
                        }
                    }

                    break;
                }
                break;
            case sf::Event::MouseWheelMoved:
                if (selectedTile != nullptr)
                {
                    selectedOffset += event.mouseWheel.delta;
                    int maxOffset = selectedTile->GetHaresCount() - HARE_PANELS;
                    if (maxOffset < 0) maxOffset = 0;
                    if (selectedOffset < 0) selectedOffset = 0;
                    if (selectedOffset > maxOffset)selectedOffset = maxOffset;
                }
                break;
            }
        }
        if (!runSimulation) continue;

        window.clear();
        window.draw(bg);

        for (int i = 0; i < TILES_SIZE; i++)
        {
            for (int j = 0; j < TILES_SIZE; j++)
            {
                tiles[i][j]->DrawTile(&window);
            }
        }

        if (selectedTile != nullptr)
        {
            int hareCount = selectedTile->GetHaresCount();

            for (int i = 0; i < HARE_PANELS; i++)
            {
                int harePointer = selectedOffset + i;

                if (harePointer < hareCount)
                {
                    Hare* h = selectedTile->GetHare(harePointer);

                    if (h != nullptr)
                    {
                        descPanels[i]->DrawHareDesc(h, &window);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        window.draw(dayCounterText);
        window.display();
    }

    
    std::cout << "Ending simulation" << std::endl;

    for (int i = 0; i < TILES_SIZE; i++)
    {
        for (int j = 0; j < TILES_SIZE; j++)
        {
            for (int k = 0; k < tiles[i][j]->GetHaresCount(); k++)
            {
                tiles[i][j]->GetHare(k)->WriteHareToDatabase(db);
            }
        }
    }

    sqlite3_close(db);

    window.close();
    
    std::cout << "End" << std::endl;
    return 0;
}

void PrepareDatabase(sqlite3* db)
{

    //tabela symulacje
    std::string sqlStatement = "CREATE TABLE IF NOT EXISTS Simulation (id INTEGER PRIMARY KEY, date INTEGER)";
    if (sqlite3_exec(db, sqlStatement.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        std::cerr << "Błąd przy tworzeniu tabeli Simulation: " << sqlite3_errmsg(db) << std::endl;
    }
    else 
    {
        std::time_t currentTime = std::time(nullptr);
        std::string curDate = std::to_string(static_cast<int>(currentTime));

        sqlStatement = "INSERT INTO Simulation (date, scientist_id) VALUES ("+curDate+",0)";
        if (sqlite3_exec(db, sqlStatement.c_str(), 0, 0, 0) != SQLITE_OK)
        {
            std::cerr << "Błąd przy dodawaniu wartości do Simulation: " << sqlite3_errmsg(db) << std::endl;
        }
        else 
        {
            std::string sqlStatement = "SELECT * FROM Simulation ORDER BY id DESC LIMIT 1";
            sqlite3_stmt* stmt;

            if (sqlite3_prepare_v2(db, sqlStatement.c_str(), -1, &stmt, NULL) == SQLITE_OK)
            {
                // Przetwarzaj wyniki zapytania
                if (sqlite3_step(stmt) == SQLITE_ROW)
                {
                    simulationID = (sqlite3_column_int(stmt, 0)+1);
                    std::cout << "Simulation number: " << simulationID << std::endl;
                }

                sqlite3_finalize(stmt);
            }
            else
            {
                std::cerr << "Błąd przy przygotowywaniu zapytania: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    //tabela pracownikow
    sqlStatement = "CREATE TABLE IF NOT EXISTS Scientists (id INTEGER PRIMARY KEY, name VARCHAR(20), surname VARCHAR(30), degree VARCHAR(40))";
    if (sqlite3_exec(db, sqlStatement.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        std::cerr << "Błąd przy tworzeniu tabeli Scientists: " << sqlite3_errmsg(db) << std::endl;
    }

    //tabela uniwersytetow
    sqlStatement = "CREATE TABLE IF NOT EXISTS Universities (id INTEGER PRIMARY KEY, name VARCHAR(50), domain VARCHAR(50))";
    if (sqlite3_exec(db, sqlStatement.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        std::cerr << "Błąd przy tworzeniu tabeli Universities: " << sqlite3_errmsg(db) << std::endl;
    }

    //tabela zajecy
    sqlStatement = "CREATE TABLE IF NOT EXISTS Hares (id INTEGER PRIMARY KEY, simulation INTEGER, simulationID INTEGER, age INTEGER, fenotype0 INTEGER, fenotype1 INTEGER, gender INTEGER, mother INTEGER, father INTEGER, isAlive INTEGER)";
    if (sqlite3_exec(db, sqlStatement.c_str(), 0, 0, 0) != SQLITE_OK) 
    {
        std::cerr << "Błąd przy tworzeniu tabeli Hares: " << sqlite3_errmsg(db) << std::endl;
    }

    //tabela laczoca pracownika z symulacja i przypisujaca dla ktorego uniwersytetu
    sqlStatement = "CREATE TABLE IF NOT EXISTS Personsimulation (id INTEGER PRIMARY KEY, simulationID INTEGER, personID INTEGER, uniID INTEGER)";
    if (sqlite3_exec(db, sqlStatement.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        std::cerr << "Błąd przy tworzeniu tabeli Personsimulation: " << sqlite3_errmsg(db) << std::endl;
    }
    else 
    {
        std::string insertSql = "INSERT INTO Personsimulation (simulationID,personID,uniID)"
            "VALUES ("+std::to_string(GetSimulationID()) +",0,0)";

        if (sqlite3_exec(db, insertSql.c_str(), 0, 0, 0) != SQLITE_OK)
        {
            std::cerr << "Błąd przy dodawaniu naukowca do obserwacji: " << sqlite3_errmsg(db) << std::endl;
            std::cerr << insertSql << std::endl;
        }
    }
}

void PrintTest(sqlite3* db)
{
    std::string sqlStatement = "SELECT * FROM Simulation ORDER BY id";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlStatement.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        // Przetwarzaj wyniki zapytania
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            int person = sqlite3_column_int(stmt, 1);

            // Wyświetl dane zająca
            std::cout << "ID: " << id << "Who: " << person << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        std::cerr << "Błąd przy przygotowywaniu zapytania: " << sqlite3_errmsg(db) << std::endl;
    }
}