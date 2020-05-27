#include <iostream>
#include<bits/stdc++.h>
using namespace std;

/**
this code generate an undirected graph with specified number of nodes and a an edge densities as a presetage
with coast range specified compute the average path length from the first node to all other nodes
in the generated graph with two different edge dinsity 20% and 40%

the output is as follows:
          with diensity of :0.2 Average Path Length :6.5
		  with diensity of :0.4 Average Path Length :3.08

I have learned how to use lambda function reviewed on STL mainly priority queue get more familiar with graph
and its representation and got more familiar with OOP in general.
I also learned how to deal with probabilities and how to be able to deal with them.
*/
class graph{
public:


    ///this constractor generates a random graph
    graph(int graphSize = 50,double edgeDinsity = 0.2,int minCoast = 1,int maxCoast = 10)
    {
        srand(time(0));
        this->graphList.resize(graphSize); ///resize the graph list to be able to contain current graph size
        this->VertexInEdges.resize(graphSize);
        this->vertexOutEdges.resize(graphSize);


        for(int i = 0;  i < graphSize ; i++)
        {
            for(int j = i ; j < graphSize ; j++)///loop throught the graph nodes to add edges for each node
                if(j != i&&  (rand()%101) <= edgeDinsity*100)
                    {
                        this->graphEdges++; ///update number of edges added
                        int cost = rand()%(maxCoast-minCoast+1) + minCoast;///coast will equal some probabilty function
                        graphList[i].push_back({cost,j});///push back new edge to the list
                        graphList[j].push_back({cost,i});

                        ///update number of edges for each vertex
                        vertexOutEdges[i]++;
                        VertexInEdges[i]++;
                        vertexOutEdges[j]++;
                        VertexInEdges[j]++;
                    }

        }
        ///fill graph class variables
        this->graphVertix = graphSize;
    }

    graph(vector<vector<tuple<int,int> > > const  initList)
    {
        this->graphList = initList;
        this->graphVertix = initList.size();
        this->graphEdges = 0;
        int indx = 0 ;
        this->vertexOutEdges.resize(this->getVertics());
        this->VertexInEdges.resize(this->getVertics());
        for(auto adjNodes : initList)
        {
            for(auto node : adjNodes)
            {
                int vertex;
                tie(std::ignore,vertex) = node;
               // cout<<vertex<<endl;
                this->VertexInEdges[vertex]++;

            }
            this->vertexOutEdges[indx++] = adjNodes.size();
            this->graphEdges += adjNodes.size();
        }

    }
    ///this constractor generate a graph from a given graph (deep copy)
    graph(const graph& g)
    {
        ///deep copy from another graph
        this->graphList = g.getGraphList();
        this->graphVertix = g.getVertics();
        this->graphEdges = g.getEdges();
        this->vertexOutEdges.resize(g.getVertics());
        this->VertexInEdges.resize(g.getVertics());
        for(int i = 0 ; i < g.getVertics() ; i++)
        {
            this->VertexInEdges[i] = g.getVertexInEdges(i);
            this->vertexOutEdges[i] = g.getVertexOutEdges(i);
        }

    }


    int getVertexInEdges(int pos)const
    {
        return this->VertexInEdges[pos];
    }

    int getVertexOutEdges(int pos) const
    {
        return this->vertexOutEdges[pos];
    }

     ///return graph list
    vector< vector<tuple<int,int> > > getGraphList() const
    {
        return this->graphList;
    }



    ///return the number of vertics in the graph
    int getVertics() const
    {
        return this->graphVertix;
    }


    ///return number of graph edges
    int getEdges() const
    {
        return this->graphEdges;
    }



    ///function to print graph
    void print()
    {
        int currentNode = 0 ;///keep record of current node that being visited
        for(auto &it : this->graphList)
        {
            cout<<currentNode<<"-->";///print current node
            for(auto node : it)
            {
                int coast,vertex;
                tie(coast,vertex) = node;
                cout<<"("<<vertex<<","<<coast<<")";
            }///for each node print it's adjacent nodes and their coast
           cout<<endl;  ///new line for next node

           currentNode++;///increase counter to indicate next node

        }
    }

    ///this function check for graph connectivity [full connected graph]
    bool isConnected(int sourceNode = 0)
    {
        queue<int> q; ///create queue to hold current lvl nodes
        q.push(sourceNode); ///push source node to the queue
        int visNodes = 1; ///add the first node to visited node counter
        int vis[this->graphVertix] = {0}; ///create a visited array to mark visited nodes
        vis[sourceNode] = 1;           ///mark current node as visited
        ///implement DFS until all node are visited or the queue is empty
        while(!q.empty() && visNodes < this->graphVertix)
        {
            int currentNode = q.front();
            q.pop();
            for(auto const& adjNode : this->graphList[currentNode])
            {
                int vertex ;
                tie(ignore,vertex) = adjNode;
                if(!vis[vertex])///check if the node already visited
                {
                    visNodes++;
                    q.push(vertex);
                    vis[vertex] = 1;
                }

            }
        }
        ///return 1 if connected
       // cout<<visNodes<<" "<<this->graphVertix<<endl;
        return (visNodes == this->graphVertix);

    }

    vector<int> shortestPath(int sourceNode)
    {
        priority_queue<tuple<int,int> > pq;
        static vector<int> coast(this->graphVertix,INT_MAX); ///array to hold the coast value for each node
        int vis[this->graphVertix] = {0};                    ///array to mark nodes in the closed set
        pq.push({0,sourceNode});                             ///push first node to the queue with coast zero
        coast[sourceNode] = 0;                               ///update coast array for the first node
        while(!pq.empty())
        {
            int currentNode;
            tie(std::ignore,currentNode) = pq.top();
            pq.pop();
            for(auto const& adjNode : graphList[currentNode])///iterate through the connectivity list of current node
            {
            int disNode , edgeCoast;
            tie(edgeCoast,disNode) = adjNode;
            if(vis[disNode] == 0) ///if adj node not visited yet
                pq.push({edgeCoast,disNode}); ///push it to the pq

            ///if the current node coast greater than the coast from the current node update coast
            if(coast[currentNode]+ edgeCoast < coast[disNode])
                coast[disNode] =  coast[currentNode]+edgeCoast;

            vis[currentNode] = 1; ///add node to closed set
            }

        }
        return coast;
    }

    ///minmum spanning tree using prims algorithem
    int minmumSpanningTree(int initNode = 0)
    {
        if(this->isConnected())
        {
            int MSTcoast = 0; ///variable to hold the MSTcoast
            int vis[this->graphVertix] = {0};
            vis[initNode] = 1;
            int visVertex = 1;

            ///priority queue to hold coast sourceNode and disNode
            priority_queue<tuple<int,int,int>,vector<tuple<int,int,int>>,greater<tuple<int,int,int>> > pq;
            int coast,disNode;
            int sourceNode = initNode;
            ///loop through node neighbours
            for(auto const adjNode : this->graphList[initNode])
            {
                tie(coast,disNode) = adjNode;
                pq.push({coast,sourceNode,disNode});
                                  //  cout<<"ft "<<sourceNode<<" "<<disNode<<" "<<coast<<endl;

            }
            while(!pq.empty() && visVertex <this->graphVertix)
            {
                tie(coast,sourceNode,disNode) = pq.top();
                pq.pop();
                if(!vis[disNode])
                {
                   // cout<<"MST "<<sourceNode<<" "<<disNode<<" "<<coast<<endl;
                    sourceNode = disNode;
                    vis[sourceNode] = 1;
                    MSTcoast += coast;
                    visVertex++;
                    for(auto const adjNode : this->graphList[sourceNode])
                    {
                        tie(coast,disNode) = adjNode;
                        if(!vis[disNode])
                        {
                             pq.push({coast,sourceNode,disNode});
                        }



                    }
                }

            }
            return MSTcoast;
        }
        else
        {
            return -1;
        }
    }

private:
int graphVertix; ///number of graph vertix [nodes]
int graphEdges; ///number of graph edges
vector<int> VertexInEdges; ///hold the number of in edges for each vertex
vector<int> vertexOutEdges;    ///hold the number of out edges for each vertex
vector< vector<tuple<int,int> > > graphList; ///this is the baisic data structure that holds the graph representation as a list
};


void week3Assigment()
{
     double diensity[2] = {0.2,0.4}; /// aray of disired edy diensity
    int graphSize = 50;             ///required graph size
    int minDis = 1;                 ///min distant for edge
    int maxDis = 10;                ///max distant for edge

    for(int i = 0 ; i < 2 ; i++)///loop throght desired diensities
    {
        double overallAverage = 0;///give the total average of multiple runs
        int runTimes = 100; ///number of run times
        for(int k =0 ; k < runTimes ; k++)
        {
            graph g(graphSize,diensity[i],minDis,maxDis);///create new graph
            double shortestPathAverage =0;     ///variable to hold average path length from node 0 to all other nodes in graph

            vector<int> a = g.shortestPath(0);  ///vector to hold coast array returned from

            for(int j =1 ; j < graphSize ; j++)  ///accumlate the coast to reach each node from node 0
            {
                shortestPathAverage += (a[j] == INT_MAX)?0:a[j];///check for no connectivity
            }
            overallAverage += (shortestPathAverage /graphSize);
        }


        ///print out result
            cout<<"with diensity of :"<<diensity[i]<<" Average Path Length :"<<overallAverage/runTimes<<endl;
    }

}

void week4Assigment()
{
ifstream infile;
infile.open("testFile.txt");
int vertx;
infile>>vertx;
int sourceNode = 0,disNode,coast;
vector<vector<tuple<int,int> > > graphList (vertx);
while(infile>>sourceNode>>disNode>>coast)
{
    graphList[sourceNode].push_back({coast,disNode});
}

graph g(graphList);

cout<<"MST : "<<g.minmumSpanningTree()<<endl;

}

int main()
{
    //week3Assigment();
    week4Assigment();
    /*
     graph g(5,0.5);
     g.print();
     cout<<"hi"<<endl;
    if(g.isConnected())
        cout<<"Y\n";
    else
        cout<<"No\n";
        cout<<"MST : "<<g.minmumSpanningTree()<<endl;
     for(int i = 0 ; i < 10 ; i++)
        cout<<i<<"->>"<<g.getVertexInEdges(i)<<endl;
    */
    return 0;
}
