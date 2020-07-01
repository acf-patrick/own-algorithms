#include <algorithm>
#include <iostream>
#include <vector>

class Robot
{
public:
	Robot(std::vector<std::string>, std::vector<int>, 
		 std::vector<int> end = std::vector<int> ());
	/**
	* @param command : la commande à executer
	* @return paire, il suffira d'ajouter cette paire aux coordonnées de l'objet pour obtenir sa nouvelle position
	*/
	std::vector<int> bind(char);
	/* Execute la commande envoyée en paramètre et retourne true si le mouvement peut être éffectué*/
	bool move(char);
	/* Execute toute une serie de commande cycliquement. Retourne vraie si l'objet est arrivé à destination */
	bool execute(std::string);
	/* true : l'objet est arrivé */
	bool arrived();
	int x, y;
private:
	// position de départ
	int s_x, s_y;
	// positinon d'arrivée
	int t_x, t_y;
	// bah... le labyrinthe
	std::vector<std::string> labyrinth;
};
class Solver
{
public:
	Solver(std::vector<std::string>, std::vector<int>, std::vector<int>);
	/* Affiche tout simplement le labyrinthe */
	void maze()
	{
		for (int i=0; i<(int)labyrinth.size(); ++i)
			std::cout << labyrinth[i] << std::endl;
	}
	/* toutes les commandes possibles vont être stocker dans cette variable .
	*  Il suffira de choisir la plus courte comme output
	*/
	std::vector<std::string> path;
private:
	bool stop;
	std::vector<std::string> labyrinth;
	/* Parcour le labyrinthe, à la recherche d'une combinaison de commandes valide */
	void parcourir(Robot, std::string commands = "");
	char inverse(char);
	std::string node(int, int);
};

std::string TzWccS6LabyrinthNavigation(std::vector<std::string> labyrinth,
	std::vector<int> start, std::vector<int> end)
{
	Solver solver(labyrinth, start, end);
	solver.maze();
	std::string shortest(8, '0');
	for (int i=0; i<(int)solver.path.size(); ++i)
		if (solver.path[i].size() < shortest.size())
			shortest = solver.path[i];
	return shortest;
}

int main()
{
	std::string tab[]={".......",
					   "...*.*.",
					   "*....*.",
					   "...*...",
					   ".......",
					   ".**.*..",
					   ".**...."};
	std::vector<std::string> lab;
	for (int i=0; i<7; ++i)
		lab.push_back(tab[i]);
	std::vector<int> start, end(2, 0);
	start.push_back(6);
	start.push_back(6);
	std::cout << TzWccS6LabyrinthNavigation(lab, start, end) << std::endl;
	return 0;
}

Solver::Solver(std::vector<std::string> lab, std::vector<int> start, std::vector<int> end):
	stop(false), labyrinth(lab)
{
	parcourir(Robot(lab, start, end));
}
void Solver::parcourir(Robot robot, std::string commands)
{
	int s(commands.size());
	char command = (s?commands[s-1]:'\0');
	while (robot.move(command))
	{
		if (stop) return;
		Robot tmp(robot);
		tmp.execute(commands);
		if (tmp.arrived())
		{
			if (std::find(path.begin(), path.end(), commands) != path.end())
				stop = true;
			else
				path.push_back(commands);
			return;
		}
		if (commands.size() > 7)
			return;
		std::string cs(node(robot.x, robot.y));
		for (int i=0; i<cs.size(); ++i)
		{
			if (cs[i] == command or cs[i] == inverse(command))
				continue;
			std::string cmd(commands);
			cmd.append(1, cs[i]);
			parcourir(robot, cmd);
		}
		commands.append(1, command);
	}
}
char Solver::inverse(char command)
{
	char ret;
	switch (command)
	{
	case 'L':
		ret = 'R';
		break;
	case 'R':
		ret = 'L';
		break;
	case 'D':
		ret = 'U';
		break;
	case 'U':
		ret = 'D';
		break;
	default: ;
	}
	return ret;
}
std::string Solver::node(int x, int y)
{
	std::vector<int> position;
	position.push_back(x);
	position.push_back(y);
	std::string commands("ULDR"), ret;
	for (int i=0; i<4; ++i)
		if (Robot(labyrinth, position).move(commands[i]))
			ret.append(1, commands[i]);
	return ret;
}
Robot::Robot(std::vector<std::string> lab, std::vector<int> start, std::vector<int> end):
	s_x(start[0]), s_y(start[1]),
	labyrinth(lab)
{
	if (!end.empty())
	{
		t_x = end[0];
		t_y = end[1];
	}
	else
	{
		t_x = s_x;
		t_y = s_y;
	}
	x = s_x;
	y = s_y;
}
std::vector<int> Robot::bind(char command)
{
	std::vector<int> ret;
	switch (command)
	{
	case 'U':
		ret.push_back(-1);
		ret.push_back(0);
		return ret;
	case 'D':
		ret.push_back(1);
		ret.push_back(0);
		return ret;
	case 'L':
		ret.push_back(0);
		ret.push_back(-1);
		return ret;
	case 'R':
		ret.push_back(0);
		ret.push_back(1);
		return ret;
	// ne devrait pas arriver mais évite les erreurs
	default: ;
	}
	return ret;
}
bool Robot::move(char command)
{
	if (command == '\0') return true;
	int tmp_x(x), tmp_y(y);
	std::vector<int> diff(bind(command));
	tmp_x += diff[0];
	tmp_y += diff[1];
	if (tmp_x>=0 and tmp_y>=0 and 
		tmp_x<(int)labyrinth.size() and tmp_y<(int)labyrinth[0].size())
		if (labyrinth[tmp_x][tmp_y] != '*')
		{
			x = tmp_x;
			y = tmp_y;
			return true;
		}
	return false;
}
bool Robot::execute(std::string commands)
{
	if (commands == "")
		return false;
	while (true)
	{
		for (int i=0; i<(int)commands.size(); ++i)
		{
			if (!move(commands[i]))
				return false;
			if (arrived())
				return true;
			if (x == s_x and y == s_y)
				return false;
		}
	}
	return false;
}
bool Robot::arrived() { return (x == t_x and y == t_y); }