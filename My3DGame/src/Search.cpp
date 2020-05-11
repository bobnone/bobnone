#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

struct GraphNode
{
	//Adjacency list
	std::vector<GraphNode*> adjacent_;
};
struct Graph
{
	//A graph contains nodes
	std::vector<GraphNode*> nodes_;
};
struct WeightedEdge
{
	//Which nodes are connected by this edge?
	struct WeightedGraphNode* from_;
	struct WeightedGraphNode* to_;
	//Weight of this edge
	float weight_;
};
struct WeightedGraphNode
{
	std::vector<WeightedEdge*> edges_;
};
struct WeightedGraph
{
	std::vector<WeightedGraphNode*> nodes_;
};
struct GBFSScratch
{
	const WeightedEdge* parentEdge_ = nullptr;
	float heuristic_ = 0.0f;
	bool inOpenSet_ = false;
	bool inClosedSet_ = false;
};

using GBFSMap = std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

struct AStarScratch
{
	const WeightedEdge* parentEdge_ = nullptr;
	float heuristic_ = 0.0f;
	float actualFromStart_ = 0.0f;
	bool inOpenSet_ = false;
	bool inClosedSet_ = false;
};

using AStarMap = std::unordered_map<const WeightedGraphNode*, AStarScratch>;

float computeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b)
{
	return 0.0f;
}
bool aStar(const WeightedGraph& g, const WeightedGraphNode* start, const WeightedGraphNode* goal, AStarMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;
	//Set current node to start, and mark in closed set
	const WeightedGraphNode* current = start;
	outMap[current].inClosedSet_ = true;
	do
	{
		//Add adjacent nodes to open set
		for(const WeightedEdge* edge : current->edges_)
		{
			const WeightedGraphNode* neighbor = edge->to_;
			//Get scratch data for this node
			AStarScratch& data = outMap[neighbor];
			//Only check nodes that aren't in the closed set
			if(!data.inClosedSet_)
			{
				if(!data.inOpenSet_)
				{
					//Not in the open set, so parent must be current
					data.parentEdge_ = edge;
					data.heuristic_ = computeHeuristic(neighbor, goal);
					//Actual cost is the parent's plus cost of traversing edge
					data.actualFromStart_ = outMap[current].actualFromStart_ + edge->weight_;
					data.inOpenSet_ = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					//Compute what new actual cost is if current becomes parent
					float newG = outMap[current].actualFromStart_ + edge->weight_;
					if (newG < data.actualFromStart_)
					{
						//Current should adopt this node
						data.parentEdge_ = edge;
						data.actualFromStart_ = newG;
					}
				}
			}
		}
		//If open set is empty, all possible paths are exhausted
		if(openSet.empty())
		{
			break;
		}
		//Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(), [&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			//Calculate f(x) for nodes a/b
			float fOfA = outMap[a].heuristic_ + outMap[a].actualFromStart_;
			float fOfB = outMap[b].heuristic_ + outMap[b].actualFromStart_;
			return fOfA < fOfB;
		});
		//Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].inOpenSet_ = true;
		outMap[current].inClosedSet_ = true;
	}
	while(current != goal);
	//Did we find a path?
	return (current == goal) ? true:false;
}
bool gBFS(const WeightedGraph& g, const WeightedGraphNode* start, const WeightedGraphNode* goal, GBFSMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;
	//Set current node to start, and mark in closed set
	const WeightedGraphNode* current = start;
	outMap[current].inClosedSet_ = true;
	do
	{
		//Add adjacent nodes to open set
		for (const WeightedEdge* edge : current->edges_)
		{
			//Get scratch data for this node
			GBFSScratch& data = outMap[edge->to_];
			//Add it only if it's not in the closed set
			if (!data.inClosedSet_)
			{
				//Set the adjacent node's parent edge
				data.parentEdge_ = edge;
				if (!data.inOpenSet_)
				{
					//Compute the heuristic for this node, and add to open set
					data.heuristic_ = computeHeuristic(edge->to_, goal);
					data.inOpenSet_ = true;
					openSet.emplace_back(edge->to_);
				}
			}
		}
		//If open set is empty, all possible paths are exhausted
		if(openSet.empty())
		{
			break;
		}
		//Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(), [&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].heuristic_ < outMap[b].heuristic_;
		});
		//Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].inOpenSet_ = false;
		outMap[current].inClosedSet_ = true;
	}
	while(current != goal);
	//Did we find a path?
	return (current == goal) ? true:false;
}

using NodeToParentMap = std::unordered_map<const GraphNode*, const GraphNode*>;

bool bFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap)
{
	//Whether we found a path
	bool pathFound = false;
	//Nodes to consider
	std::queue<const GraphNode*> q;
	//Enqueue the first node
	q.emplace(start);
	while(!q.empty())
	{
		//Dequeue a node
		const GraphNode* current = q.front();
		q.pop();
		if(current == goal)
		{
			pathFound = true;
			break;
		}
		//Enqueue adjacent nodes that aren't already in the queue
		for(const GraphNode* node : current->adjacent_)
		{
			//If the parent is null, it hasn't been enqueued
			//(except for the start node)
			const GraphNode* parent = outMap[node];
			if(parent == nullptr && node != start)
			{
				//Enqueue this node, setting its parent
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}
	return pathFound;
}
void testBFS()
{
	Graph g;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			GraphNode* node = new GraphNode;
			g.nodes_.emplace_back(node);
		}
	}
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			GraphNode* node = g.nodes_[i * 5 + j];
			if (i > 0)
			{
				node->adjacent_.emplace_back(g.nodes_[(i - 1) * 5 + j]);
			}
			if (i < 4)
			{
				node->adjacent_.emplace_back(g.nodes_[(i + 1) * 5 + j]);
			}
			if (j > 0)
			{
				node->adjacent_.emplace_back(g.nodes_[i * 5 + j - 1]);
			}
			if (j < 4)
			{
				node->adjacent_.emplace_back(g.nodes_[i * 5 + j + 1]);
			}
		}
	}
	NodeToParentMap map;
	bool found = bFS(g, g.nodes_[0], g.nodes_[9], map);
	std::cout << found << '\n';
}
void testHeuristic(bool useAStar)
{
	WeightedGraph g;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			WeightedGraphNode* node = new WeightedGraphNode;
			g.nodes_.emplace_back(node);
		}
	}
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			WeightedGraphNode* node = g.nodes_[i * 5 + j];
			if(i > 0)
			{
				WeightedEdge* e = new WeightedEdge;
				e->from_ = node;
				e->to_ = g.nodes_[(i - 1) * 5 + j];
				e->weight_ = 1.0f;
				node->edges_.emplace_back(e);
			}
			if(i < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->from_ = node;
				e->to_ = g.nodes_[(i + 1) * 5 + j];
				e->weight_ = 1.0f;
				node->edges_.emplace_back(e);
			}
			if(j > 0)
			{
				WeightedEdge* e = new WeightedEdge;
				e->from_ = node;
				e->to_ = g.nodes_[i * 5 + j - 1];
				e->weight_ = 1.0f;
				node->edges_.emplace_back(e);
			}
			if(j < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->from_ = node;
				e->to_ = g.nodes_[i * 5 + j + 1];
				e->weight_ = 1.0f;
				node->edges_.emplace_back(e);
			}
		}
	}
	bool found = false;
	if(useAStar)
	{
		AStarMap map;
		found = aStar(g, g.nodes_[0], g.nodes_[9], map);
	}
	else
	{
		GBFSMap map;
		found = gBFS(g, g.nodes_[0], g.nodes_[9], map);
	}
	std::cout << found << '\n';
}
struct GameState
{
	//(For tic-tac-toe, array of board)
	enum SquareState { Empty, X, O };
	SquareState board_[3][3];
};
struct GTNode
{
	//Children nodes
	std::vector<GTNode*> children_;
	//State of game
	GameState state_;
};
void genStates(GTNode* root, bool xPlayer)
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(root->state_.board_[i][j] == GameState::Empty)
			{
				GTNode* node = new GTNode;
				root->children_.emplace_back(node);
				node->state_ = root->state_;
				node->state_.board_[i][j] = xPlayer ? GameState::X : GameState::O;
				genStates(node, !xPlayer);
			}
		}
	}
}
float getScore(const GameState& state)
{
	//Are any of the rows the same?
	for(int i = 0; i < 3; i++)
	{
		bool same = true;
		GameState::SquareState v = state.board_[i][0];
		for(int j = 1; j < 3; j++)
		{
			if(state.board_[i][j] != v)
			{
				same = false;
			}
		}
		if(same)
		{
			if(v == GameState::X)
			{
				return 1.0f;
			}
			else
			{
				return -1.0f;
			}
		}
	}
	//Are any of the columns the same?
	for(int j = 0; j < 3; j++)
	{
		bool same = true;
		GameState::SquareState v = state.board_[0][j];
		for(int i = 1; i < 3; i++)
		{
			if(state.board_[i][j] != v)
			{
				same = false;
			}
		}
		if(same)
		{
			if(v == GameState::X)
			{
				return 1.0f;
			}
			else
			{
				return -1.0f;
			}
		}
	}
	//What about diagonals?
	if(((state.board_[0][0] == state.board_[1][1]) && (state.board_[1][1] == state.board_[2][2])) || ((state.board_[2][0] == state.board_[1][1]) && (state.board_[1][1] == state.board_[0][2])))
	{
		if(state.board_[1][1] == GameState::X)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}
	}
	//We tied
	return 0.0f;
}

float minPlayer(const GTNode* node);

float maxPlayer(const GTNode* node)
{
	//If this is a leaf, return score
	if(node->children_.empty())
	{
		return getScore(node->state_);
	}
	float maxValue = -std::numeric_limits<float>::infinity();
	//Find the subtree with the maximum value
	for(const GTNode* child : node->children_)
	{
		maxValue = std::max(maxValue, minPlayer(child));
	}
	return maxValue;
}
float minPlayer(const GTNode* node)
{
	//If this is a leaf, return score
	if(node->children_.empty())
	{
		return getScore(node->state_);
	}
	float minValue = std::numeric_limits<float>::infinity();
	//Find the subtree with the minimum value
	for(const GTNode* child : node->children_)
	{
		minValue = std::min(minValue, maxPlayer(child));
	}
	return minValue;
}
const GTNode* minimaxDecide(const GTNode* root)
{
	//Find the subtree with the maximum value, and save the choice
	const GTNode* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	for(const GTNode* child : root->children_)
	{
		float v = minPlayer(child);
		if(v > maxValue)
		{
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta);

float alphaBetaMax(const GTNode* node, float alpha, float beta)
{
	//If this is a leaf, return score
	if(node->children_.empty())
	{
		return getScore(node->state_);
	}
	float maxValue = -std::numeric_limits<float>::infinity();
	//Find the subtree with the maximum value
	for(const GTNode* child : node->children_)
	{
		maxValue = std::max(maxValue, alphaBetaMin(child, alpha, beta));
		if(maxValue >= beta)
		{
			//Beta prune
			return maxValue;
		}
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}
float alphaBetaMin(const GTNode* node, float alpha, float beta)
{
	//If this is a leaf, return score
	if(node->children_.empty())
	{
		return getScore(node->state_);
	}
	float minValue = std::numeric_limits<float>::infinity();
	//Find the subtree with the minimum value
	for(const GTNode* child : node->children_)
	{
		minValue = std::min(minValue, alphaBetaMax(child, alpha, beta));
		if(minValue <= alpha)
		{
			//Alpha prune
			return minValue;
		}
		beta = std::min(minValue, beta);
	}
	return minValue;
}
const GTNode* alphaBetaDecide(const GTNode* root)
{
	//Find the subtree with the maximum value, and save the choice
	const GTNode* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for(const GTNode* child : root->children_)
	{
		float v = alphaBetaMin(child, maxValue, beta);
		if(v > maxValue)
		{
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}
void testTicTac()
{
	GTNode* root = new GTNode;
	root->state_.board_[0][0] = GameState::O;
	root->state_.board_[0][1] = GameState::Empty;
	root->state_.board_[0][2] = GameState::X;
	root->state_.board_[1][0] = GameState::X;
	root->state_.board_[1][1] = GameState::O;
	root->state_.board_[1][2] = GameState::O;
	root->state_.board_[2][0] = GameState::X;
	root->state_.board_[2][1] = GameState::Empty;
	root->state_.board_[2][2] = GameState::Empty;
	genStates(root, true);
	const GTNode* choice = alphaBetaDecide(root);
	std::cout << choice->children_.size();
}