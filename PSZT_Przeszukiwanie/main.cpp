#include <limits>
#include "PathFinder.h"
#include "GraphGenerator.h"
#include <iomanip>
#include <ctime>
#include "Generator.h"


/*
void main(int argc, char** argv)
{
//generateGraph(10); // creates graph definition i generated_graph.txt
//Graph g("generated_graph.txt");
Graph g;
generate(g, 10, 40);
bool eOK = true;
int E = 0;
cout << "NODES: \n";
for (auto i = g.nodes.begin(); i != g.nodes.end(); ++i)
{
Node& n = i->second;
printf("Node: %s (%lf, %lf)\n", n.name.c_str(), n.point.x, n.point.y);
cout << "connected with: \n";
for (auto e = n.edges.begin(); e != n.edges.end(); e++)
{
Node* o = e->otherNode(&n);
float dist = n.distance(*(e->otherNode(&n)));
if (n.distance(*(e->otherNode(&n))) > e->cost()) {
eOK = false;
cout << "\nFAULTY!: " << n.name << " " << o->name <<" d = "<<dist<<" cost: "<<e->cost()<< endl;
}
printf("\t%s cost: %lf\n", e->otherNode(&n)->name.c_str(), e->cost());
E++;
}
}
cout << "|V| = " << g.nodes.size() << " |E| = " << E/2 << endl;;
if (eOK)
cout << "\nEDGES OK\n" << endl;
else
cout << "\nEDGES not OK\n" << endl;
//A* search
cout << "A* search: \n";
PathFinder pf(&g);
Node& src = g.nodes["P1"];
Node& trgt = g.nodes["P8"];
//Time A*
clock_t begin = clock();
Path& sp = pf.A_star(src, trgt);
clock_t end = clock();
double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

for (auto i = sp.begin(); i != sp.end(); i++)
{
Node* n = *i;
cout << n->name << " - ";
}
cout << "\nTotal cost: " << sp.total_cost;
cout << "\nExecution time: " << elapsed_secs<<endl;


//Brute search
cout << "\nBFS search: \n";
PathFinder pf_brute(&g);
//Time BFS
begin = clock();
Path& sp_brute = pf_brute.shortestPathBFS(src, trgt);
end = clock();
elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

for (auto i = sp_brute.begin(); i != sp_brute.end(); i++)
{
Node* n = *i;
cout << n->name << " - ";
}
cout << "\nTotal cost: " << sp_brute.total_cost;
cout << "\nExecution time: " << elapsed_secs<<endl;

getchar(); getchar();
return;
}
*/

string in_fname, out_fname;
const int STANDARD = 0, TIME_TESING = 1, GENERATOR = 2, HELP = 3, UNSET = -1;
int mode = UNSET; // Execution mode
bool useA_star = true, useBrute = false; // flags to choose whitch algorithm will be used to find shortest path
bool useVisualization = false; // flag to enable visualization
int nodesCount, edgesCount, testStep = 100; //  generator paramerters
void parse_args(int argc, char** argv);
void parseGeneratorParams(int, char**, int&);
void printHelp();
void executeStandardMode();
void executeGeneratorMode();
void executeTimeTests();
void printGraph(Graph&);
void printResult(Path&, Node&, Node&);


void main(int argc, char** argv)
{
	try {
		if (argc < 2)
			throw exception("\nToo few arguments!\n");
		else
		{
			parse_args(argc, argv);
			if (mode == UNSET)
				mode = STANDARD;
		}
	}
	catch (exception ex) {
		cout << ex.what() << endl;
		cout << "Run with -help to see available parameters\n";
		return;
	}
	try {
		switch (mode)
		{
		case HELP:
		{
			printHelp();
			return;
		}
		case STANDARD:
		{
			executeStandardMode();
			return;
		}
		case GENERATOR:
		{
			executeGeneratorMode();
			return;
		}
		case TIME_TESING:
		{
			executeTimeTests();
			return;
		}
		}
	}
	catch (exception ex) {
		cout << ex.what() << endl;
	}
	return;
}




/**
Standard mode - read graph from file
*/
void executeStandardMode()
{
	if (in_fname.empty()) {
		cout << "Supply input file.\n";
		return;
	}

	Graph g(in_fname);
	printGraph(g);
	string source_name, target_name;
	cout << "Enter source point name: ";
	cin >> source_name;
	cout << "Enter target point name: ";
	cin >> target_name;
	auto sit = g.nodes.find(source_name);
	auto tit = g.nodes.find(target_name);
	if (sit == g.nodes.end()) {
		cout << "Point " << source_name << " not found in the graph\n";
		return;
	}
	if (tit == g.nodes.end()) {
		cout << "Point " << target_name << " not found in the graph\n";
		return;
	}
	if (useA_star) {
		cout << "A* search: \n";
		PathFinder pf(&g);
		//Time A*
		clock_t begin = clock();
		Path& sp = pf.A_star(sit->second, tit->second);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		printResult(sp, sit->second, tit->second);
		cout << "\nExecution time: " << elapsed_secs << endl;
		if (useVisualization && !useBrute)
		{
			while (1)
			{
				visualize(g, sp);
				cout << "Input source node (q to exit): ";
				cin >> source_name;
				if (source_name == "q")
					exit(0);
				cout << "Input destination node (q to exit): ";
				cin >> target_name;
				if (target_name == "q")
					exit(0);
				sit->second = g.nodes[source_name];
				tit->second = g.nodes[target_name];
				begin = clock();
				sp = pf.A_star(sit->second, tit->second);
				end = clock();
				elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << "A* search: \n";
				printResult(sp, sit->second, tit->second);
				cout << "\nExecution time: " << elapsed_secs << endl;
			}
		}
	}

	if (useBrute) {
		//Brute search
		cout << "\nBFS search: \n";
		PathFinder pf_brute(&g);
		//Time BFS
		clock_t begin = clock();
		Path& sp_brute = pf_brute.shortestPathBFS(sit->second, tit->second);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		printResult(sp_brute, sit->second, tit->second);
		cout << "\nExecution time: " << elapsed_secs << endl;
		if (useVisualization && !useA_star)
		{
			while (1)
			{
				visualize(g, sp_brute);
				cout << "Input source node (q to exit): ";
				cin >> source_name;
				if (source_name == "q")
					exit(0);
				cout << "Input destination node (q to exit): ";
				cin >> target_name;
				if (target_name == "q")
					exit(0);
				sit->second = g.nodes[source_name];
				tit->second = g.nodes[target_name];
				begin = clock();
				sp_brute = pf_brute.shortestPathBFS(sit->second, tit->second);
				end = clock();
				elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << "\nBFS search: \n";
				printResult(sp_brute, sit->second, tit->second);
				cout << "\nExecution time: " << elapsed_secs << endl;
			}
		}
	}
}

/**
GENERATOR MODE - generates random graph with given parameters and finds shortest path beetween random points
*/
void executeGeneratorMode()
{
	srand(time(NULL));
	Graph g;
	generate_graph(g, nodesCount, edgesCount);
	string base_name("P");
	int s = (rand() % nodesCount);
	int t = (rand() % nodesCount);
	if (s == t)
		t = (s + 1) % nodesCount;
	string source_name = base_name + to_string(s);
	string target_name = base_name + to_string(t);

	Node& source = g.nodes[source_name];
	Node& target = g.nodes[target_name];

	if (useA_star) {
		cout << "A* search: \n";
		PathFinder pf(&g);
		//Time A*
		clock_t begin = clock();
		Path& sp = pf.A_star(source, target);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		printResult(sp, source, target);
		cout << "\nExecution time: " << elapsed_secs << endl;
		if (useVisualization && !useBrute)
		{
			while (1)
			{
				visualize(g, sp);
				cout << "Input source node (q to exit): ";
				cin >> source_name;
				if (source_name == "q")
					exit(0);
				cout << "Input destination node (q to exit): ";
				cin >> target_name;
				if (target_name == "q")
					exit(0);
				source = g.nodes[source_name];
				target = g.nodes[target_name];
				begin = clock();
				sp = pf.A_star(source, target);
				end = clock();
				elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << "A* search: \n";
				printResult(sp, source, target);
				cout << "\nExecution time: " << elapsed_secs << endl;
			}
		}

	}

	if (useBrute) {
		//Brute search
		cout << "\nBFS search: \n";
		PathFinder pf_brute(&g);
		//Time BFS
		clock_t begin = clock();
		Path& sp_brute = pf_brute.shortestPathBFS(source, target);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		printResult(sp_brute, source, target);
		cout << "\nExecution time: " << elapsed_secs << endl;
		if (useVisualization)
		{
			while (1)
			{
				visualize(g, sp_brute);
				cout << "Input source node (q to exit): ";
				cin >> source_name;
				if (source_name == "q")
					exit(0);
				cout << "Input destination node (q to exit): ";
				cin >> target_name;
				if (target_name == "q")
					exit(0);
				source = g.nodes[source_name];
				target = g.nodes[target_name];
				begin = clock();
				sp_brute = pf_brute.shortestPathBFS(source, target);
				end = clock();
				elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << "\nBFS search: \n";
				printResult(sp_brute, source, target);
				cout << "\nExecution time: " << elapsed_secs << endl;
			}
	
		}
	}
}


/**

*/
void executeTimeTests()
{
	srand(time(NULL));
	const int REPS_PER_SIZE = 10; // # of reps for mean time calculation
	const int node_step = testStep; //  test step supplied by user default 100
	int max_edges = (nodesCount * (nodesCount - 1)) / 2;
	const float edge_ratio = (float)edgesCount / (float)max_edges; // edge ratio in original test set 
	double A_time = 0.0, brute_time = 0.0; //mean time of test for recent size
	cout << "Edge ratio: " << edge_ratio << endl;
	if (useA_star && useBrute)
		cout << "|V|" << "\t|E|" << "\tA* time" << "\tBrute time" << endl;
	else if (useA_star || useBrute)
		cout << "|V|" << "\t|E|" << "\tExecution time" << endl;
	while (A_time < 60.0 && brute_time < 60.0) // set mean_time limit for 1min
	{
		double A_time_sum = 0.0, brute_time_sum = 0.0;
		for (int i = 0; i < REPS_PER_SIZE; i++)
		{
			Graph g;
			generate_graph(g, nodesCount, edgesCount);
			string base_name("P");
			int s = (rand() % nodesCount);
			int t = (rand() % nodesCount);
			if (s == t)
				t = (s + 1) % nodesCount;
			string source_name = base_name + to_string(s);
			string target_name = base_name + to_string(t);

			Node& source = g.nodes[source_name];
			Node& target = g.nodes[target_name];

			if (useA_star) {
				clock_t begin = clock();
				PathFinder pf(&g);
				Path& sp = pf.A_star(source, target);
				clock_t end = clock();
				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				A_time_sum += elapsed_secs;
			}
			if (useBrute) {
				clock_t begin = clock();
				PathFinder pf(&g);
				Path& sp = pf.shortestPathBFS(source, target);
				clock_t end = clock();
				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				brute_time_sum += elapsed_secs;
			}
		}
		//Calculate mean execution time
		if (useA_star)
			A_time = A_time_sum / (float)REPS_PER_SIZE;
		if (useBrute)
			brute_time = brute_time_sum / (float)REPS_PER_SIZE;

		if (useA_star && useBrute)
			cout << nodesCount << "\t" << edgesCount << "\t" << A_time << "\t" << brute_time << endl;
		else if (useA_star)
			cout << nodesCount << "\t" << edgesCount << "\t" << A_time << endl;
		else if (useBrute)
			cout << nodesCount << "\t" << edgesCount << "\t" << brute_time << endl;

		// Update generator params
		nodesCount += node_step;
		int new_max_roads = (nodesCount * (nodesCount - 1)) / 2;
		edgesCount = (int)(new_max_roads * edge_ratio);
	}
}

void printGraph(Graph& g)
{
}

void printResult(Path& sp, Node& source, Node& target)
{
	cout << "\nShortest path from " << source.name << " to " << target.name << ": \n";
	if (!sp.isValid())
	{
		cout << "Path not found.\n";
		return;
	}

	for (auto p = sp.path.begin(); p != sp.path.end(); ++p)
	{
		Node* n = *p;
		cout << n->name << " - ";
	}
	cout << "\nTotal cost: " << sp.total_cost << endl;
}


/*
Prints help message on std out
*/
void printHelp()
{
	cout << "Opcje uruchumienia programu :\n" <<
		"-in <nazwa_pliku> \t-plik z danymi wejsciowymi\n" <<
		"-v \t-uzyj wizualizacji\n" <<
		"-gen \t-losowo generuje instancje problemu i rozwi�zuje go. Parmetry generatora:\n" <<
		"\t-n <liczba_punktow>\n" <<
		"\t-e <liczba_krawedzi>\n" <<
		"-time <parametry_generatora> [-s <step>]\t- pomiary czasu dla losowo generownych instancji poroblemu\n\tGenerowane grafy maja podobna gestosc do poczatkowego grafu.\n" <<
		"-A \t-nie korzystaj z algorytmu A*\n" <<
		"+B \t-korzystaj z algorytmu brutalnego domyslnie tylko A*\n";
}

/*
Parse command line arguments and set context
*/
void parse_args(int argc, char** argv)
{
	int i = 1; // argv[0] is program name
	while (i<argc)
	{
		if (strcmp(argv[i], "-in") == 0)
		{// Input file
			if (i == argc - 1)
				throw exception("Input file not defined!\n");
			else {
				i++;
				in_fname = string(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-gen") == 0)
		{ // Generator mode
			if (i == argc - 1)
				throw exception("Missing generator parameters!\n");
			else {
				i++;
				int start_i = i;
				parseGeneratorParams(argc, argv, i);
				if (i - start_i < 4)
					throw exception("Too few parameters for generator!\n");
				mode = GENERATOR;
			}
		}
		else if (strcmp(argv[i], "-time") == 0)
		{ // Time testing mode
			mode = TIME_TESING;
			i++;
			int start_i = i;
			parseGeneratorParams(argc, argv, i);
			if (i - start_i < 4)
				throw exception("Too few parameters for generator!\n");
		}
		else if (strcmp(argv[i], "-help") == 0)
		{ // Help mode
			mode = HELP;
		}
		else if (strcmp(argv[i], "-A") == 0)
		{ // dont use A*
			useA_star = false;
		}
		else if (strcmp(argv[i], "+B") == 0)
		{ // use brute force
			useBrute = true;
		}
		else if (strcmp(argv[i], "-v") == 0)
		{ // use visualization
			useVisualization = true;
		}
		else { // Unknown parameter
			cout << argv[i];
			throw exception("Unknown parameter!\n");
		}
		i++;
	}
}
/*
Parses generator params
*/
void parseGeneratorParams(int argc, char** argv, int& i)
{
	while (i < argc)
	{
		if (strcmp(argv[i], "-n") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				nodesCount = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-e") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				edgesCount = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-s") == 0)
		{
			if (i == argc - 1)
				throw exception("Step value!\n");
			else
			{
				i++;
				testStep = atoi(argv[i]);
			}
		}
		else { // arg is not a param - exit
			i--; // let arg be parsed by command parser
			return;
		}
		i++;
	}
}
