#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <sstream>
#include <stack>
#include <list>
#include <set>
#include <map>

using namespace std;

const int size = 16;
//const int 4 = 4;
vector<vector<set<int>>> choice(size, vector<set<int>>(size, set<int>()));

class Coord
{
public:
    int row;
    int col;
    int val;
};

vector<vector<int>> grid(size, vector<int>(size));
//int grid[size][size];
bool Solved = false;

bool isvalid(pair<int, int> coord, int num)
{
    int row = coord.first;
    int col = coord.second;
    int RSS = row - row % 4;
    int CSS = col - col % 4;
    //check row
    for (int c = 0; c < size; c++)
    {
        if (num == grid[row][c])
        {
            return false;
        }
    }
    //check col
    for (int r = 0; r < size; ++r)
    {
        if (grid[r][col] == num)
        {
            return false;
        }
    }
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            if (grid[r + RSS][c + CSS] == num)
            {
                return false;
            }
        }
    }
    return true;
}

void getChoices()
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            set<int> &vals = choice[row][col];
            if (grid[row][col] != 0)
            {
                continue;
            }
            for (int num = 1; num <= size; ++num)
            {
                if (isvalid({row, col}, num))
                {
                    vals.insert(num);
                }
            }
        }
    }
}

void Display()
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            if (grid[row][col] > 9)
            {
                char out = grid[row][col] + 55;
                cout << out;
            }
            else
            {
                cout << grid[row][col];
            }

            if (col != size - 1)
            {
                cout << " ";
            }
        }
        cout << endl;
    }
}

bool checkEmptyCoord(int &r, int &c)
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            if (grid[row][col] == 0)
            {
                r = row;
                c = col;
                return true;
            }
        }
    }
    return false;
}

void updateChoices(int &row, int &col, int num)
{
    grid[row][col] = num;
    choice[row][col].clear();
    for (int r = 0; r < size; ++r)
    {
        choice[row][r].erase(num);
        choice[r][col].erase(num);
    }
    int CSS = (col / 4) * 4;
    int RSS = (row / 4) * 4;

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            choice[RSS + r][CSS + c].erase(num);
        }
    }
}

void hiddenSingles()
{
    //rows
    bool foundHS;
    do
    {
        foundHS = false;
        for (int row = 0; row < size; ++row)
        {
            map<int, Coord> hSMap;
            for (int col = 0; col < size; ++col)
            {
                set<int> &choiceSet = choice[row][col];
                if (grid[row][col])
                {
                    continue;
                }
                for (int num : choiceSet)
                {
                    if (hSMap.find(num) == hSMap.end())
                    {
                        Coord curr;
                        curr.row = row;
                        curr.col = col;
                        curr.val = 1;
                        hSMap[num] = curr;
                    }
                    else
                    {
                        hSMap[num].val++;
                    }
                }
            }
            for (auto i : hSMap)
            {
                if (i.second.val == 1)
                {
                    updateChoices(i.second.row, i.second.col, i.first);
                    foundHS = true;
                }
            }
        }
        //cols
        for (int col = 0; col < size; ++col)
        {
            map<int, Coord> hsMap;
            for (int row = 0; row < size; ++row)
            {
                set<int> &vals = choice[row][col];
                if (grid[row][col])
                {
                    continue;
                }
                for (int num : vals)
                {
                    Coord curr;
                    curr.row = row;
                    curr.col = col;
                    if (hsMap.find(num) == hsMap.end())
                    {
                        curr.val = 1;
                        hsMap.insert({num, curr});
                    }
                    else
                    {
                        hsMap.at(num).val++;
                    }
                }
            }
            for (auto i : hsMap)
            {
                if (i.second.val == 1)
                {
                    updateChoices(i.second.row, i.second.col, i.first);
                    foundHS = true;
                }
            }
        }
        //box
        for (int box = 0; box < size; box++)
        {
            int rowS = (box / 4) * 4;
            int colS = (box % 4) * 4;
            map<int, Coord> hsMap;
            for (int row = 0; row < 4; ++row)
            {
                for (int col = 0; col < 4; ++col)
                {
                    set<int> &vals = choice[row + rowS][col + colS];
                    if (grid[row + rowS][col + colS])
                    {
                        continue;
                    }
                    for (int num : vals)
                    {
                        if (hsMap.find(num) == hsMap.end())
                        {
                            Coord curr;
                            curr.row = row + rowS;
                            curr.col = colS + col;
                            curr.val = 1;
                            hsMap.insert({num, curr});
                        }
                        else
                        {
                            hsMap.at(num).val++;
                        }
                    }
                }
            }
            for (auto i : hsMap)
            {
                if (i.second.val == 1)
                {
                    updateChoices(i.second.row, i.second.col, i.first);
                    foundHS = true;
                }
            }
        }
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                if (choice[row][col].size() == 1)
                {
                    int num = *choice[row][col].begin();
                    updateChoices(row, col, num);
                    foundHS = true;
                }
            }
        }
    } while (foundHS);
}
void Solve()
{
    hiddenSingles();
    int r;
    int c;

    if (checkEmptyCoord(r, c) == false)
    {
        Solved = true;
        Display();
        return;
    }

    vector<vector<set<int>>> newChoice = choice;
    vector<vector<int>> newGrid = grid;

    for (int val : newChoice[r][c])
    {
        updateChoices(r, c, val);
        Solve();

        if (!Solved)
        {
            grid[r][c] = 0;
            choice = newChoice;
            grid = newGrid;
        }
        else
        {
            return;
        }
    }
}

int main()
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            char inp;
            cin >> inp;

            if (inp >= 65 && inp <= 90)
            {
                grid[row][col] = inp - 55;
            }
            else
            {
                grid[row][col] = inp - '0';
            }
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    getChoices();
    cout<<endl;
    Solve();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " millisecond" << endl;

    if (!Solved)
    {
        cout << "No Solution" << endl;
    }

    return 0;
}
