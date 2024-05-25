#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

class Graph {
public:
	Graph(int n) : n(n), adjList(n), inDegree(n, 0) {}

	void addEdge(int winner, int loser) {
		adjList[winner].push_back(loser);
		inDegree[loser]++;
	}

	std::vector<int> findChampions() {
		std::vector<int> champions;
		for (int i = 0; i < n; ++i) {
			if (inDegree[i] == 0) {
				champions.push_back(i);
			}
		}
		return champions;
	}

	void printGraph() const {
		for (int i = 0; i < n; ++i) {
			std::cout << "Team " << i << " defeated: ";
			for (int neighbor : adjList[i]) {
				std::cout << neighbor << " ";
			}
			std::cout << std::endl;
		}
	}

private:
	int n; // количество команд
	std::vector<std::vector<int>> adjList; // список смежности
	std::vector<int> inDegree; // массив входящих степеней вершин
};

int main() {
	int n = 5; // количество команд
	std::vector<std::pair<int, int>> matches = { {0, 1}, {1, 2}, {2, 3}, {4, 3} };

	Graph tournamentGraph(n);

	for (const auto& match : matches) {
		tournamentGraph.addEdge(match.first, match.second);
	}

	std::vector<int> champions = tournamentGraph.findChampions();

	std::cout << "Champions: ";
	for (int champion : champions) {
		std::cout << champion << " ";
	}
	std::cout << std::endl;

	tournamentGraph.printGraph();

	return 0;
}