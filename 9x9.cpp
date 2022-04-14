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
#include <chrono>

using namespace std;

const int  size = 9;

vector<vector<set<int>>> choice(size, vector<set<int>>(size, set<int>()));

class Coord
{
public:
    int row;
    int col;
    int val;
};

int grid[size][size];
bool Solved = false;

bool isvalid(pair<int, int> coord, int num)
{
    int row = coord.first;
    int col = coord.second;

    int RSS = row - row % 3;
    int CSS = col - col % 3;
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

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
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
            cout << grid[row][col];

            if (col != 8)
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
        choice[r][col].erase(num);
        choice[row][r].erase(num);
    }

    int CSS = (col / 3) * 3;
    int RSS = (row / 3) * 3;

    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            choice[RSS + r][CSS + c].erase(num);
        }
    }
}

void hiddenSingles()
{
    //rows
    bool found;
    do
    {
        found = false;
        
        for (int row = 0; row < size; ++row)
        {
            //hSMap.clear();
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
                        hSMap[num].val++;
                }
            }

            for (auto i : hSMap)
            {
                if (i.second.val == 1)
                {
                    updateChoices(i.second.row, i.second.col, i.first);
                    found = true;
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
                    found = true;
                }
            }
        }

        //box
        for (int box = 0; box < size; box++)
        {
            int RSS = (box / 3) * 3;
            int CSS = (box % 3) * 3;

            map<int, Coord> hsMap;

            for (int row = 0; row < 3; ++row)
            {
                for (int col = 0; col < 3; ++col)
                {
                    set<int> &vals = choice[row + RSS][col + CSS];

                    if (grid[row + RSS][col + CSS])
                        continue;

                    for (int num : vals)
                    {
                        if (hsMap.find(num) == hsMap.end())
                        {
                            Coord curr;
                            curr.row = row + RSS;
                            curr.col = CSS + col;
                            curr.val = 1;
                            hsMap.insert({num, curr});
                        }
                        else
                            hsMap.at(num).val++;
                    }
                }
            }

            for (auto i : hsMap)
            {
                if (i.second.val == 1)
                {
                    updateChoices(i.second.row, i.second.col, i.first);
                    found = true;
                }
            }
        }

        for (int r = 0; r < size; r++)
        {
            for (int c = 0; c < size; c++)
            {
                if (choice[r][c].size() == 1)
                {
                    grid[r][c] = *choice[r][c].begin();
                    found = true;
                }
            }
        }
    } while (!found);
}
void Solve()
{
    int r;
    int c;

    if (checkEmptyCoord(r, c) == false)
    {
        Solved = true;
        Display();
        return;
    }

    for (int val : choice[r][c])
    {
        if (isvalid({r, c}, val))
        {
            grid[r][c] = val;

            Solve();

            if (!Solved)
            {
                grid[r][c] = 0;
            }
        }
    }
}
int main()
{
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            cin >> grid[row][col];
        }
    }

    
    auto start = std::chrono::high_resolution_clock::now();
    getChoices();
    hiddenSingles();
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



