//
// Ai.cpp for zappy in /home/gastal_r/rendu/Zappy/Ai/src
//
// Made by
// Login   <gastal_r>
//
// Started on  Wed Jun 21 13:20:37 2017
// Last update Wed Jun 21 13:20:39 2017
//

#include      "Ai.hpp"
#include      "Utils.hpp"

Ai::Ai(int port, char* machine) noexcept
  : Communication(port, machine),
  _riseUpConditions({
  {1, {{Ai::Properties::NB_PLAYER, 1}, {Ai::Properties::LINEMATE, 1}, {Ai::Properties::DERAUMERE, 0}, {Ai::Properties::SIBUR, 0}, {Ai::Properties::MENDIANE, 0}, {Ai::Properties::PHIRAS, 0}, {Ai::Properties::THYSTAME, 0}}},
  {2, {{Ai::Properties::NB_PLAYER, 2}, {Ai::Properties::LINEMATE, 1}, {Ai::Properties::DERAUMERE, 1}, {Ai::Properties::SIBUR, 1}, {Ai::Properties::MENDIANE, 0}, {Ai::Properties::PHIRAS, 0}, {Ai::Properties::THYSTAME, 0}}},
  {3, {{Ai::Properties::NB_PLAYER, 2}, {Ai::Properties::LINEMATE, 2}, {Ai::Properties::DERAUMERE, 0}, {Ai::Properties::SIBUR, 1}, {Ai::Properties::MENDIANE, 0}, {Ai::Properties::PHIRAS, 2}, {Ai::Properties::THYSTAME, 0}}},
  {4, {{Ai::Properties::NB_PLAYER, 4}, {Ai::Properties::LINEMATE, 1}, {Ai::Properties::DERAUMERE, 1}, {Ai::Properties::SIBUR, 2}, {Ai::Properties::MENDIANE, 0}, {Ai::Properties::PHIRAS, 1}, {Ai::Properties::THYSTAME, 0}}},
  {5, {{Ai::Properties::NB_PLAYER, 4}, {Ai::Properties::LINEMATE, 1}, {Ai::Properties::DERAUMERE, 2}, {Ai::Properties::SIBUR, 1}, {Ai::Properties::MENDIANE, 3}, {Ai::Properties::PHIRAS, 0}, {Ai::Properties::THYSTAME, 0}}},
  {6, {{Ai::Properties::NB_PLAYER, 6}, {Ai::Properties::LINEMATE, 1}, {Ai::Properties::DERAUMERE, 2}, {Ai::Properties::SIBUR, 3}, {Ai::Properties::MENDIANE, 0}, {Ai::Properties::PHIRAS, 1}, {Ai::Properties::THYSTAME, 0}}},
  {7, {{Ai::Properties::NB_PLAYER, 6}, {Ai::Properties::LINEMATE, 2}, {Ai::Properties::DERAUMERE, 2}, {Ai::Properties::SIBUR, 2}, {Ai::Properties::MENDIANE, 2}, {Ai::Properties::PHIRAS, 2}, {Ai::Properties::THYSTAME, 1}}},
  }),
  _currentItems({ {Ai::Properties::LINEMATE, 0}, {Ai::Properties::DERAUMERE, 0}, {Ai::Properties::SIBUR, 0}, {Ai::Properties::MENDIANE, 0}, {Ai::Properties::PHIRAS, 0}, {Ai::Properties::THYSTAME, 0} }),
  _currentLevel(1)
{}

void      Ai::start(void) noexcept
{
  try
  {
    lookForResources();
  }
  catch (const std::exception &error)
  {

  }
}

bool      Ai::needResources(void) noexcept
{
  if (_currentItems[Ai::Properties::LINEMATE] >= _riseUpConditions[_currentLevel][Ai::Properties::LINEMATE]
    && _currentItems[Ai::Properties::DERAUMERE] >= _riseUpConditions[_currentLevel][Ai::Properties::DERAUMERE]
    && _currentItems[Ai::Properties::SIBUR] >= _riseUpConditions[_currentLevel][Ai::Properties::SIBUR]
    && _currentItems[Ai::Properties::MENDIANE] >= _riseUpConditions[_currentLevel][Ai::Properties::MENDIANE]
    && _currentItems[Ai::Properties::PHIRAS] >= _riseUpConditions[_currentLevel][Ai::Properties::PHIRAS]
    && _currentItems[Ai::Properties::THYSTAME] >= _riseUpConditions[_currentLevel][Ai::Properties::THYSTAME])
    {
      return (false);
    }
  return (true);
}

bool      Ai::broadcast(void) noexcept
{
  return (true);
}

bool      Ai::lookForResources(void) noexcept
{
  std::string rawView("player,,,thystame,,food,,,,,,linemate,,,,,");

  std::vector<std::vector<Ai::Properties>>  view;

  size_t posCase = 0;
  std::string currentCase;
  while ((posCase = rawView.find(",")) != std::string::npos)
  {
    currentCase = rawView.substr(0, posCase);
    size_t posItem = 0;
    std::vector<Ai::Properties> caseItems;
    if ((posItem = currentCase.find(" ")) != std::string::npos)
    {
      while ((posItem = currentCase.find(" ")) != std::string::npos)
      {
        std::string currentItem(currentCase.substr(0, posItem));
        caseItems.push_back(Utils::stringToEnum(currentItem));
        currentCase.erase(0, posItem + 1);
      }
    }
    caseItems.push_back(Utils::stringToEnum(currentCase));
    view.push_back(caseItems);
    rawView.erase(0, posCase + 1);
  }
  size_t caseNbr(0);
  std::cout << "Look command result: " << std::endl;
  for (auto & it : view)
  {
    std::cout << "[" << caseNbr++ << "]";
    for (auto & it2 : it)
    {
      std::cout << " " << (int) it2 << " ";
    }
    std::cout << std::endl;
  }
  int resourceCase = findNeededResourceCase(view);
  if (resourceCase == -1)
    return (false);
  calculatePath(resourceCase);
  return (true);
}

int      Ai::findNeededResourceCase(const std::vector<std::vector<Ai::Properties>>  &view) noexcept
{
  size_t caseNbr = 0;
  for (auto & currentCase : view)
  {
    for (auto & currentItem : currentCase)
    {
      if ((currentItem == Ai::Properties::LINEMATE) &&  _currentItems[Ai::Properties::LINEMATE] < _riseUpConditions[_currentLevel][Ai::Properties::LINEMATE])
      {
        _objectToTake = Ai::Properties::LINEMATE;
          return (caseNbr);
      }
      if ((currentItem == Ai::Properties::DERAUMERE) &&  _currentItems[Ai::Properties::DERAUMERE] < _riseUpConditions[_currentLevel][Ai::Properties::DERAUMERE])
      {
        _objectToTake = Ai::Properties::DERAUMERE;
          return (caseNbr);
      }
      if ((currentItem == Ai::Properties::SIBUR) &&  _currentItems[Ai::Properties::SIBUR] < _riseUpConditions[_currentLevel][Ai::Properties::SIBUR])
      {
        _objectToTake = Ai::Properties::SIBUR;
          return (caseNbr);
      }
      if ((currentItem == Ai::Properties::MENDIANE) &&  _currentItems[Ai::Properties::MENDIANE] < _riseUpConditions[_currentLevel][Ai::Properties::MENDIANE])
      {
        _objectToTake = Ai::Properties::MENDIANE;
          return (caseNbr);
      }
      if ((currentItem == Ai::Properties::PHIRAS) &&  _currentItems[Ai::Properties::PHIRAS] < _riseUpConditions[_currentLevel][Ai::Properties::PHIRAS])
      {
        _objectToTake = Ai::Properties::PHIRAS;
          return (caseNbr);
      }
      if ((currentItem == Ai::Properties::THYSTAME) &&  _currentItems[Ai::Properties::THYSTAME] < _riseUpConditions[_currentLevel][Ai::Properties::THYSTAME])
      {
        _objectToTake = Ai::Properties::THYSTAME;
          return (caseNbr);
      }
    }
    caseNbr++;
  }
  return (-1);
}
#include <chrono>
#include <thread>
void      Ai::calculatePath(int resourceCase) noexcept
{
  int   offsetX = 0;
  int   offsetY = 0;
  int   nextLineOffset = 0;
  int   curMaxOffsetX = 0;
  int   curMinOffsetX = 0;
  int   currentCase = 0;

  std::cout << "ressourceCase => " << resourceCase << std::endl << std::endl;
  _path.clear();
  while (currentCase != resourceCase)
  {
    curMinOffsetX = offsetY * offsetY;
    curMaxOffsetX = curMinOffsetX + offsetY * 2;
    // std::cout << "curMinOffsetX: " << curMinOffsetX << std::endl;
    // std::cout << "curMaxOffsetX: " << curMaxOffsetX << std::endl;
    nextLineOffset = ((offsetY + 1) * (offsetY + 1)) + (offsetY > 0 ? offsetX + 1 : offsetX);
    std::cout << "currentCase: " << currentCase << std::endl;
    int direction = calculateDirection(resourceCase, (currentCase - 1 < curMinOffsetX ? currentCase : currentCase - 1), nextLineOffset,
      (currentCase + 1 > curMaxOffsetX ? currentCase : currentCase + 1));
    std::cout << "direction=> " << direction << std::endl;
    if (direction == 0)
    {
      offsetX -= 1;
      currentCase -= 1;
      _path.push_back(Ai::Direction::LEFT);
    }
    else if (direction == 1)
    {
      offsetY += 1;
      offsetX += 1;
      currentCase = offsetY * offsetY + offsetX;
      _path.push_back(Ai::Direction::UP);
    }
    else if (direction == 2)
    {
      offsetX += 1;
      currentCase += 1;
      _path.push_back(Ai::Direction::RIGHT);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout  << std::endl;
  }
}

int     Ai::calculateDirection(int destination, int a, int b, int c) noexcept
{
  int aLength = destination - a;
  int bLength = destination - b;
  int cLength = destination - c;

  aLength < 0 ? aLength *= -1 : 0;
  bLength < 0 ? bLength *= -1 : 0;
  cLength < 0 ? cLength *= -1 : 0;
  // std::cout << "a: " << a << "  b: " << b << "  c: " << c << std::endl;
  // std::cout << "aLength: " << aLength << "  bLength: " << bLength << "  cLength: " << cLength << std::endl;
  return ((aLength < bLength && aLength < cLength ? 0 : (bLength < aLength && bLength < cLength ? 1 : 2)));
}
