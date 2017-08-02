using System;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

public class	Link
{
	private	int	_x;
	private	int	_y;

	public int		getX()
	{
		return (_x);
	}

	public int		getY()
	{
		return (_y);
	}

	public void		set(int x, int y)
	{
		this._x = x;
		this._y = y;
	}

	public		Link()
	{
		this.set(-1, -1);
	}

	public		Link(int x)
	{
		this.set(-1, -1);
	}

	public		Link(int x, int y)
	{
		this.set(x, y);
	}
}

public class	Node
{
	private	int		_id;
	private bool	_is_gateway;
	private Link[]	_roads;
	private	int		_gtw_link;

	public int		getGtwLink()
	{
		return (this._gtw_link);
	}

	public void		setGtwLink(Node[] nodes)
	{
		int[]		links;

		links = this.getLink();
		foreach (int link in links) {
			if (nodes[link].getGateway() == true)
			{
				this._gtw_link += 1;
			}
		}
	}

	public int[]	getLink()
	{
		int[]		ret = new int[_roads.Length];
		int			i = 0;

		foreach (Link _road in _roads)
		{
			ret[i] = _road.getY();
			i++;
		}

		return (ret);
	}

	public void		setLink(Link[] roads)
	{
		int		i = 0;

		foreach (Link _road in roads)
		{
			if (_road.getX() == this._id || _road.getY() == this._id)
				i++;
		}

		this._roads = new Link[i];

		i = 0;

		foreach (Link _road in roads)
		{
			if (_road.getX() == this._id)
			{
				_roads[i] = new Link(this._id, _road.getY());
				i++;
			}
			else if (_road.getY() == this._id)
			{
				_roads[i] = new Link(this._id, _road.getX());
				i++;
			}
		}

	}

	public void		setGateway(bool xvalue)
	{
		_is_gateway = xvalue;
	}

	public bool		getGateway()
	{
		return (_is_gateway);
	}

	public int		getId()
	{
		return (_id);
	}

	public			Node()
	{
		this._id = -1;
		this._is_gateway = false;
		this._gtw_link = 0;
	}

	public			Node(int id)
	{
		this._id = id;
		this._is_gateway = false;
		this._gtw_link = 0;
	}
}

class Player
{
	static int		findStart(Node[] nodes, int[] done, int pos, int size, int[] min)
	{
		if (nodes[pos].getGateway() == true && size < min[0])
		{
			min[0] = size;
			return (pos);
		}

		done[pos] = 1;

		int[]	linked = nodes[pos].getLink();

		foreach (int next in linked) {
			if (done[next] == 0)
				return (findStart(nodes, next, size + 1, min));
		}

		return (pos);
	}

	static int		findPath(Node[] nodes, int[] done, int pos)
	{
		int[]	linked = nodes[pos].getLink();
		int		gateway_linked = nodes[pos].getGtwLink();
		int		ret;

		done[pos] = 1;

		if (gateway_linked >= 2)
		{
			return (pos);
		}
		else if (gateway_linked == 0)
		{
			return (-1);
		}
		else
		{

			foreach (int next in linked) {

				if (done[next] == 0)
				{

					ret = findPath(nodes, done, next);

					if (ret != -1 && nodes[ret].getGtwLink() >= 2)
					{
						return (ret);
					}
					else
					{
						return (pos);
					}

				}

			}

			return (pos);

		}
	}

    static void		Main(string[] args)
    {
        string[]	inputs;

		inputs = Console.ReadLine().Split(' ');

		int N = int.Parse(inputs[0]); // the total number of nodes in the level, including the gateways
        int L = int.Parse(inputs[1]); // the number of links
        int E = int.Parse(inputs[2]); // the number of exit gateways

		Link[]		roads = new Link[L];

		Node[]		nodes = new Node[N];

		int[]		done = new int[N];

		for (int i = 0; i < L; i++)
        {
            inputs = Console.ReadLine().Split(' ');
			roads[i] = new Link(int.Parse(inputs[0]), int.Parse(inputs[1]));
        }

		for (int i = 0; i < N; i++)
		{
			nodes[i] = new Node(i);
			nodes[i].setLink(roads);
		}

		for (int i = 0; i < E; i++)
        {
            int EI = int.Parse(Console.ReadLine()); // the index of a gateway node
			nodes[EI].setGateway(true);
        }

        // game loop
        while (true)
        {
            int SI = int.Parse(Console.ReadLine()); // The index of the node on which the Skynet agent is positioned this turn

            // Write an action using Console.WriteLine()
            // To debug: Console.Error.WriteLine("Debug messages...");

			for (int i = 0; i < N; i++) {
				done[i] = 0;
			}

			Console.Error.WriteLine("NODE DEBUG ------------------------------");

			foreach (Node node in nodes) {
				Console.Error.WriteLine("Node id : {0}:", node.getId());
				int[]	linkList = node.getLink();
				foreach (int lnk in linkList) {
					Console.Error.WriteLine("Is linked to {0}", lnk);
				}
			}

			Console.Error.WriteLine("LINK DEBUG ------------------------------");

			foreach (Link road in roads) {
				Console.Error.WriteLine("Road : {0} to {1}.", road.getX(), road.getY());
			}

			int		answer;

			int		start;

			start = findStart(nodes, SI);

			answer = findPath(nodes, done, start);

            // Example: 3 4 are the indices of the nodes you wish to sever the link between
            Console.WriteLine(answer);
        }
    }
}
