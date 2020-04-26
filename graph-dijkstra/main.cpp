#include <iostream>
#include<bits/stdc++.h>
using namespace std;


class graph{
public:
    graph(int graphSize = 50,double edgeDinsity = 0.2,int minCoast = 1,int maxCoast = 10)
    {
        srand(time(0));
        for(int i = 0;  i < graphSize ; i++)
        {
            vector<pair<int,int> > vii; ///vector carry node edges
            for(int j = 0 ; j < graphSize ; j++)///loop throught the graph nodes to add edges for each node
                if(j != i&&  (rand()%101) <= edgeDinsity*100)
                    {
                        this->graphEdges++; ///update number of edges added
                        int cost = rand()%(maxCoast-minCoast+1) + minCoast;///coast will equal some probabilty function
                        vii.push_back({cost,j});///push back new edge to the list
                    }

            this->graphList.push_back(vii); ///add node info to the graph data structure
        }
        ///fill graph class variables
        this->graphVertix = graphSize;

    }

    ///return the number of vertics in the graph
    int vertics() const
    {
        return this->graphVertix;
    }
    ///return number of graph edges
    int edges() const
    {
        return this->graphEdges;
    }

    ///function to print graph
    void print()
    {
        int currentNode = 0 ;///keep record of current node that being visited
        for(vector< vector<pair<int,int> > >::const_iterator it = this->graphList.begin() ; it < this->graphList.end() ;++it)
        {
            cout<<currentNode<<"-->";///print current node

           for_each(it->begin(),it->end(),
            [](auto const & inner_it){
            cout<<"("<<inner_it.second<<","<<inner_it.first<<")";
            });///for each node print it's adjacent nodes and their coast
           cout<<endl;  ///new line for next node

           currentNode++;///increase counter to indicate next node

    }
    }

    vector<int> shortestPath(int sourceNode,int distinationNode)
    {
        priority_queue<pair<int,int> > pq;
        static vector<int> coast(this->graphVertix,INT_MAX); ///array to hold the coast value for each node
        int vis[this->graphVertix] = {0};                    ///array to mark nodes in the closed set
        pq.push({0,sourceNode});                             ///push first node to the queue with coast zero
        coast[sourceNode] = 0;                               ///update coast array for the first node
        while(!pq.empty())
        {
            pair<int,int> node = pq.top();
            pq.pop();
            for(auto const& adjNode : graphList[node.second])///iterate through the connectivity list of current node
            {
            if(vis[adjNode.second] == 0) ///if adj node not visited yet
                pq.push(adjNode); ///push it to the pq

            ///if the current node coast greater than the coast from the current node update coast
            if(coast[node.second]+adjNode.first < coast[adjNode.second])
                coast[adjNode.second] =  coast[node.second]+adjNode.first;

            vis[node.second] = 1; ///add node to closed set
            }

        }
        return coast;
    }
private:
int graphVertix; ///number of graph vertix [nodes]
int graphEdges; ///number of graph edges
vector< vector<pair<int,int> > > graphList; ///this is the baisic data structure that holds the graph representation as a list
};
int main()
{
    double diensity[2] = {0.2,0.4}; /// aray of disired edy diensity
    int graphSize = 50;             ///required graph size
    int minDis = 1;                 ///min distant for edge
    int maxDis = 10;                ///max distant for edge

    for(int i = 0 ; i < 2 ; i++)///loop throght desired diensities
    {
        graph g(graphSize,diensity[i],minDis,maxDis);///create new graph
        double shortestPathAverage =0;     ///variable to hold average path length from node 0 to all other nodes in graph

        vector<int> a = g.shortestPath(0,1);  ///vector to hold coast array returned from

        for(int j =1 ; j < graphSize ; j++)  ///accumlate the coast to reach each node from node 0
        {
            shortestPathAverage += a[j];
        }

        ///print out result
            cout<<"with diensity of :"<<diensity[i]<<" Average Path Length :"<<shortestPathAverage /graphSize<<endl;
    }

    return 0;
}
