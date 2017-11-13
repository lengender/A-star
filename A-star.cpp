#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
using namespace std;

//the numbers of max vertices
const int MAX_NUM = 100;

//struct of edge
typedef struct EdgeNode{
    char start; 
    char end; 
    int weight;
    struct EdgeNode *next;
}EdgeNode;

//struct of vertex
typedef struct VertexNode
{
    char name;
    int x, y;
    EdgeNode* head;
}VertexNode;

//the number of vertices and edges
int vexNum = 0;
int edgeNum = 0;

//adjex 
VertexNode adjList[MAX_NUM];

//priority queue element
typedef struct Element
{
    char vex;
    char parent;
    int x;
    int y;
    int gval;
    double fval;
    
}Element;

class PQueue
{
public:
    PQueue(){
        size = 0;
        data = new Element[MAX_NUM];
    }

    //insert a new element to priority queue
    void push(Element num)
    {
        data[size] = num;
    
        size++;
        up(size - 1);
    }

    Element front()
    {
        Element res = data[0];
        //delete the first element
        data[0] = data[size - 1];
        size--;
        down(0);
        return res;
    }

    //judge element is or not in priority queue
    int inPriQueue(char vex)
    {
        for(int i = 0; i < size; ++i)
        {
            if(data[i].vex == vex)
                return i;
        }

        return -1;
    }

    //adjust element's fval
    void adjustFval(Element e, int index)
    {
        data[index] = e;
        for(int i = (size - 1) / 2; i >= 0; --i)
            down(i);
    }

    ~PQueue()
    {
        if(data)
            delete[] data;
        data = NULL;
    }

private:
    void down(int index)
    {
        assert(index >= 0);
        Element tmp = data[index];
        index = index * 2 + 1;
        while(index < size)
        {
            if(index + 1 < size && data[index].fval > data[index + 1].fval)
                index++;
            if(data[index].fval > tmp.fval) break;
            else
            {
                data[(index - 1) / 2] = data[index];
                index = index * 2 + 1;
            }
        }
        data[(index - 1) / 2] = tmp;
    }

    void up(int index)
    {
        assert(index < size);
        Element tmp = data[index];
        while(index > 0 && tmp.fval < data[(index - 1) / 2].fval)
        {
            data[index] = data[(index - 1) / 2];
            index = (index - 1) / 2;
        }
        data[index] = tmp;
    }

public:
    bool isEmpty()
    {
        return size == 0;
    }

    bool smallerFval(int index, int fval)
    {
        if(data[index].fval > fval)
            return true;
    
        return false;
    }

public:
    Element *data;
    int size;
};


//get the index of adjList by the vertex
int getIndexByVertex(VertexNode adjList[], int num, char start)
{
    for(int i = 0; i < num; ++i)
    {
        if(adjList[i].name == start)
            return i;
    }

    return -1;
}

//judge the vertex is or not visited
int isVisited(Element closed[], int num, char name)
{
    for(int i = 0; i < num; ++i)
    {
        if(closed[i].vex == name)
            return i;
    }

    return -1;
}

//compute two vertices of Euclidean distance
double distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) * 1.0 + (y1 - y2) * (y1 - y2) * 1.0);
}

//print shortest path
void printPath(Element closed[], int num, char start, char end)
{
    char path[MAX_NUM];
    int size = 0;

    for(; ;){
        for(int i = 0; i < num; ++i){
            if(closed[i].vex == end)
            {
                path[size++] = end;
                end = closed[i].parent;
                break;
            }
        }

        if(end == 0)
            break;
    };

    cout << "the shortest path is: ";
    for(int i = size - 1; i >= 0; --i)
    {
        cout << path[i] << " ";
    }
    cout << endl;
}

//A start algorithm to compute shortest path
int AstarShortestPath(char start, char end)
{
    int shortestPath = 0;
    PQueue priQ;
    Element closed[MAX_NUM];
    int num = 0, pos = 0;
    
    //get the index by start and end vertex   
    int s_index = getIndexByVertex(adjList, vexNum, start);
    int e_index = getIndexByVertex(adjList, vexNum, end);

    //init a element by start vertex
    Element e;
    e.vex = start;
    e.x = adjList[s_index].x;
    e.y = adjList[s_index].y;
    e.parent = 0;
    e.gval = 0;
    e.fval = 0;
    priQ.push(e);  //push quque

    while(!priQ.isEmpty())
    {
        Element tmp = priQ.front();
        closed[num++] = tmp;  //add the element to closed array
        if(tmp.vex == end)   //if this is end vertex
        {
            //print path and length
            printPath(closed, num, start, end);
            cout << "the shortest path length is: " << tmp.gval << endl;
            shortestPath = tmp.gval;
            break;
        }
      
        int index = getIndexByVertex(adjList, vexNum, tmp.vex);
        EdgeNode *ptr = adjList[index].head;
        //handle the adjacent vertex
        while(ptr)
        {
            Element t;
            pos = getIndexByVertex(adjList, vexNum, ptr->end);
            t.vex = ptr->end;
            t.x = adjList[pos].x;
            t.y = adjList[pos].y;
            t.parent = ptr->start;
            t.gval = tmp.gval + ptr->weight;
            t.fval = t.gval + distance(t.x, t.y, adjList[e_index].x, adjList[e_index].y);
            
            //if this vertex has visited, continue
            if(isVisited(closed, num, ptr->end) != -1)
            {
                ptr = ptr->next;
                continue;
            }
            else
            { 
                //judge this vertex is or not in priority queue
                int pri_index = priQ.inPriQueue(ptr->end);
                if(pri_index == -1)
                {
                    priQ.push(t);
                }
                else{
                    //if it's fval is smaller, adjust the element
                    if(priQ.smallerFval(pri_index, t.fval))
                        priQ.adjustFval(t, pri_index);
                }
                ptr = ptr->next;
            }
        }
    }
    
    return shortestPath;
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "./usage filename" << endl;
        return 0;
    }

    ifstream in;
    in.open(argv[1]);

    in >> vexNum >> edgeNum;
    VertexNode tmp;

    for(int i = 0; i < vexNum; ++i){
        in >> tmp.name >> tmp.x >> tmp.y;
        tmp.head = NULL;
        adjList[i] = tmp;
    }

    //for(int i = 0; i < vexNum; ++i)
      //  cout << adjList[i].name << " " << adjList[i].x << " " << adjList[i].y << endl;

    EdgeNode eArr[MAX_NUM];
    int index = 0;
    for(int i = 0; i < edgeNum; ++i)
    {
        EdgeNode *e = new EdgeNode;
        in >> e->start >> e->end >> e->weight;
        e->next = NULL;
        eArr[i].start = e->start;
        eArr[i].end = e->end;
        eArr[i].weight = e->weight;

        index = getIndexByVertex(adjList, vexNum, e->start);
        
        if(adjList[index].head == NULL)
        {
            adjList[index].head = e;
        }
        else{
            e->next = adjList[index].head;
            adjList[index].head = e;
        }
    }

    char start, end;
    in >> start >> end;

    AstarShortestPath(start, end);
    return 0;
}
