#include <functional>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <deque>
#include <stack>
#include <functional>
#include <queue>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <vector>
#include <iostream>

using namespace std;

// some macro
typedef vector<vector<int> > vvi;
typedef vector<int>vi;
typedef pair<vvi,int> board;

map<vvi,bool> checked;
map<vvi , vvi> parent;
vvi goal = {{1,2,3},{4,5,6},{7,8,0}};
//all possible movement of empty cell "0"
int x[]={-1,+1,0,0};
int y[]={0,0,-1,+1};

// number of misplaced tiles of a board
int misplaced(vvi a)
{
    int sol = 0 ;
    for (int i = 0 ; i<3; i++) {
        for (int j = 0;j<3; j++) {
            if(a[i][j]!=3*i+j && a[i][j]!=0) sol++;
        }
    }
    return sol;
}
// compare function that makes the priority queue sort by the least number of misplaced tiles
struct cmp
{
    bool operator() (board &a, board &b)
    {
        return misplaced(a.first)<misplaced(b.first);
    }
};
// check the possibility of the cell 0 to move to i j
bool possible(int i,int j)
{
    if(i>=0 && i<=2 && j>=0 && j<=2)return true;
    return false;
}
// function to return position of empty cell
pair<int,int> empty_cell(vvi a){
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(a[i][j]==0)
            {
                return {i,j};
            }
        }
    }
    return {0,0};
}


//generale all boards from actual board
vector<vvi> all_child(vvi a)
{
    pair<int,int> pos=empty_cell(a);
    vector<vvi> children;
    
    // cheking all possible moves
    for(int move=0;move<4;move++)
    {
        int current_x = pos.first;
        int current_y = pos.second;
        vvi n = a;
        if(possible(current_x+x[move],current_y+y[move]))
        {
            swap(n[current_x+x[move]][current_y+y[move]],n[current_x][current_y]);
            children.push_back(n);
        }
    }
    return children;
}
// print a board
void print(vvi s)
{
    for(int i=0;i<3;i++){for(int j=0;j<3;j++)printf("%d ",s[i][j]);cout<<endl;}
    
}
// print recursively the path to solution from leaf to ancestor
void print_solution(vvi s)
{
    if(parent.count(s))
        print_solution(parent[s]);
    print(s);
    cout<<endl;
    return;
}
// core algorithm for A*


void process_A(vvi a, int moves=0)
{
    // initial priority queue
    priority_queue<board,vector<board>,cmp > Prioq;
    //pushing the first board
    Prioq.push(board(a,moves));
    while(!Prioq.empty())
    {
        vvi s = Prioq.top().first;
        // mark as visited
        checked[s]=1;
        Prioq.pop();
        if(s==goal)
        {
            //result and escape from function
            print_solution(s);
            return;
        }
        // generate all possible boards
        vector<vvi> ns = all_child(s);
        for(auto it : ns)
        {
            vvi temp = it;
            // extract the unvisited boards
            if(!checked[temp])
            {
                parent.insert(pair<vvi , vvi >(temp,s));
                Prioq.push(board(temp,moves+1));
            }
        }
    }
    // when there no node to process and no result
    cout<<("No solution found!");
    return;
}

//core algorithm for bfs
void process_bfs(vvi a, int moves=0)
{
    // initial queue
    queue<board> queue;
    //pushing the first board
    queue.push(board(a,moves));
    while(!queue.empty())
    {
        vvi s = queue.front().first;
        // mark as visited
        checked[s]=1;
        queue.pop();
        if(s==goal)
        {
            //result and escape from function
            print_solution(s);
            return;
        }
        // generate all possible boards
        vector<vvi> ns = all_child(s);
        for(auto it : ns)
        {
            vvi temp = it;
            // extract the unvisited boards
            if(!checked[temp])
            {
                parent.insert(pair<vvi , vvi >(temp,s));
                queue.push(board(temp,moves+1));
            }
        }
    }
    cout<<("No solution found!");
    return;
}

int main()
{
    
    
    int seeds[3] = {50,100,5};
    for (int testcase = 0; testcase<3; testcase++) {
        // clear all previous data
        checked.clear();
        parent.clear();
        cout<<endl<<"=============================="<<endl;
        cout<<"Intial matrix "<<testcase+1<<endl;
        //initial matrix
        vvi a(3,vi(3));
        vi temp = {1,2,3,4,5,6,7,8,0};
        //seed replacement
        srand(static_cast<unsigned int>(seeds[testcase]));
        // random shuffle of temp
        std::shuffle(temp.begin(), temp.end(), default_random_engine(rand()));
        
        // setting initial board from random temp
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                a[i][j] = temp[i*3+j];
                cout<<a[i][j]<<" ";
            }
            cout<<endl;
        }
        // solving
        cout<<"A star solution : " <<endl;
        process_A(a);
        checked.clear();
        parent.clear();
        cout<<"Bfs solution : " <<endl;
        process_bfs(a);
    }
}
