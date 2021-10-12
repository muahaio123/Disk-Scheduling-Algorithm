// Name: Thomas Laney, Thanh Long Le
// Team: Valorant
// Project: Disk scheduling algorithms
// we only do FIFO, SSTF, SCAN, LOOK

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

// global variables
int original_pos;	// hold the current position of the pointer
vector<int> req_original;	// hold our requests info
vector<bool> visit_original;	// is it visited or not
string direction = "right";	// change to only "right" or "left"

int randomInt(int n)	// borowed from professor LeGrand
{
	// Return a random nonnegative integer less than n.
	int r;
	if (n <= 0)
		return 0;
	
	do
	{
		r = random();
	} while (r >= INT_MAX / n * n);
	
	return r / (INT_MAX / n);
}

void outputRequests()
{
	cout << setw(7) << "arrival" << setw(11) << "position"<< endl;
	
	for (uint i = 0; i < req_original.size(); ++i)
		cout << setw(7) << i << setw(11) << req_original[i] << endl;
}

void outputResult(string name, vector<int> visit, int time, int distance)
{
	cout << "\n----------Results of " << name << "----------" << endl;
	cout << "Original pointer position: " << original_pos << endl;
	
	if (name == "SCAN" || name == "LOOK")	// additional output if it is these two direction
		cout << "Original direction: " << direction << endl;
	
	cout << "Total travel distance: " << distance << endl;
	cout << "Traveling sequence: " << endl;
	cout << original_pos << " -> ";
	for (uint i = 0; i < visit.size(); ++i)
	{
		cout << visit[i];
		
		if (i == 12)
			cout << endl;
		
		if (i != visit.size() - 1)	// output the error until it reaches the last element
			cout << " -> ";
	}
	
	cout << endl;
}

void FIFO(vector<int> req_order, vector<bool> req_visit)
{
	vector<int> visit_order;
	
	int total_time = 0, total_dist = 0, distance = 0, current_pos = original_pos;
	
	for (uint i = 0; i < req_order.size(); ++i)
	{
		req_visit[i] = true;	// set this request to have been visited
		
		// find the travel distance from the current position to the request
		distance = abs(req_order[i] - current_pos);
		
		current_pos = req_order[i];	// update the pointer to point to the current request
		
		total_dist += distance;	// update the total travel distance
		
		visit_order.push_back(req_order[i]);
	}
	
	outputResult("FIFO", visit_order, total_time, total_dist);	// output the results
}

void SSTF(vector<int> req_order, vector<bool> req_visit)
{
	vector<int> visit_order;
	
	int total_time = 0, total_dist = 0, current_pos = original_pos;
	
	for (uint i = 0; i < req_order.size(); ++i)	// do a pass for the number of all requests
	{
		int req_min = -1, min_diff = INT_MAX;
		
		for (uint j = 0; j < req_order.size(); ++j)	// find the nearest request according to position
		{
			if (req_visit[j] == false)	// only take into account those that not visited yet
			{
				int distance = abs(req_order[j] - current_pos);	// find the smallest travel distance
				if (distance < min_diff)
				{
					min_diff = distance;	// update to the smallest diff in travel distance
					req_min = j;	// remember the index of the nearest request
				}
			}
		}
		
		req_visit[req_min] = true;
		
		current_pos = req_order[req_min];	// update the pointer to point to the current request
		
		total_dist += min_diff;	// update the total travel distance
		
		visit_order.push_back(req_order[req_min]);
	}
	
	outputResult("SSTF", visit_order, total_time, total_dist);	// output the results
}

void SCAN(vector<int> req_order, vector<bool> req_visit)	// go in 1 direction till it hits the end then will go in the opposite way
{
	vector<int> visit_order;
	
	string original_dir = direction;
	
	int total_time = 0, total_dist = 0, distance = 0, current_pos = original_pos, next_pos = current_pos;
	
	// do a pass for the number of all requests (# request * 2 to take into account on its ways back, it ahs to go back to the node that it has already visited)
	for (uint i = 0; i < req_order.size() * 2; ++i)
	{
		while (find(req_order.begin(), req_order.end(), next_pos) == req_order.end())	// while the next_pos is not one of the request position
		{
			if (direction == "right")
				++next_pos;
			else if (direction == "left")
				--next_pos;
			
			if (next_pos >= 200)	// change the direction once it hits the end
				direction = "left";
			else if (next_pos <= -1)	// change the direction once it hits the start
				direction = "right";
		}
				
		uint j = 0;
		for (; j < req_order.size(); ++j)	// find the index in the req_order of the next request
			if (req_order[j] == next_pos)
				break;
			
		if (req_visit[j] == false)
		{	
			req_visit[j] = true;
			
			// find the travel distance from the current position to the request
			distance = abs(req_order[j] - current_pos);
			
			// update the current position with the request position
			current_pos = req_order[j];
			
			total_dist += distance;	// update the total travel distance
			
			visit_order.push_back(req_order[j]);
		}
		
		if (direction == "right")
			++next_pos;
		else if (direction == "left")
			--next_pos;
	}
	
	outputResult("SCAN", visit_order, total_time, total_dist);	// output the results
	direction = original_dir;	// restore the original direction
}

void LOOK(vector<int> req_order, vector<bool> req_visit)
{
	vector<int> visit_order;
	string original_dir = direction;
	
	int total_time = 0, total_dist = 0, distance = 0, current_pos = original_pos, next_pos = current_pos;
	
	for (uint i = 0; i < req_order.size() * 2; ++i)	// do a pass for the number of all requests (# request * 2 to take into account on its ways back, it ahs to go back to the node that it has already visited)
	{
		while (find(req_order.begin(), req_order.end(), next_pos) == req_order.end())	// while the next_pos is not one of the request position
		{
			if (direction == "right")
				++next_pos;
			else if (direction == "left")
				--next_pos;
			
			if (next_pos >= 200)	// move the pointer to the start of the queue
				next_pos = 0;
			else if (next_pos <= -1)	// move the pointer to the end of the queue
				next_pos = 200;
		}
		
		uint j = 0;
		for (; j < req_order.size(); ++j)	// find the index in the req_order of the next request
			if (req_order[j] == next_pos)
				break;
			
		if (req_visit[j] == false)
		{	
			req_visit[j] = true;
			
			// find the travel distance from the current position to the request
			distance = abs(req_order[j] - current_pos);
			
			// update the current position with the request position
			current_pos = req_order[j];
			
			total_dist += distance;	// update the total travel distance
			
			visit_order.push_back(req_order[j]);
		}
		
		if (direction == "right")
			++next_pos;
		else if (direction == "left")
			--next_pos;
	}
	
	outputResult("LOOK", visit_order, total_time, total_dist);	// output the results
	direction = original_dir;	// restore the original direction
}

void ask_for_position()
{
	do
	{
		cout << "Please enter the starting position of the pointer (0 <= position <= 200): ";
		cin >> original_pos;
	} while (original_pos < 0 || original_pos > 200);	// keep prompting until the desired number is entered
}

void ask_for_direction()
{
	do
	{
		cout << "Please enter the starting direction for SCAN and LOOK (\"right\" or \"left\"): ";
		cin >> direction;
	} while (direction != "right" && direction != "left");	// keep prompting until the user input "right" or "left"
}

void apply_alg()
{
	FIFO(req_original, visit_original);
	SSTF(req_original, visit_original);
	SCAN(req_original, visit_original);
	LOOK(req_original, visit_original);
}

int main()
{
	ask_for_position();
	ask_for_direction();
	
	// create a random seed for each run
	srand(time(NULL));
	
	/*----------scenario_1----------*/
	cout << "***************START SCENARIO 1***************\n";
	// create a fix sequence order and have all the algorithms work on it
	
	// the vector that contain the order of the request > -1 && < 200
	req_original = {13, 123, 59, 78, 167, 64, 25, 5, 159, 1, 145, 88, 199};
	
	// record only the request's position
	for (uint i = 0; i < req_original.size(); ++i)
		visit_original.push_back(false);
	
	outputRequests();	// output the info of requests for scenario 1
	
	// apply the algorithms
	apply_alg();
	
	cout << "****************END SCENARIO 1****************\n\n";
	/*----------scenario_1----------*/
	
	// create new requests for the below scenarios
	vector<int> new_req, reqCP = req_original;
	
	for (int i = 0; i < 10; ++i)
	{
		int num = randomInt(200);
		
		while (find(req_original.begin(), req_original.end(), num) != req_original.end())	// if this new request is unique
			num = randomInt(200);
		
		new_req.push_back(num);
	}
	// create new requests for the below scenarios
	
	/*----------scenario_2----------*/
	cout << "***************START SCENARIO 2***************\n";
	cout << "ADD 10 ITEM AT START OF QUEUE" << endl;
	
	req_original.insert(req_original.begin(), new_req.begin(), new_req.end());
	
	visit_original.clear();
	
	for (uint i = 0; i < req_original.size(); ++i)
		visit_original.push_back(false);
	
	outputRequests();	// output the info of requests for scenario 2
	
	// apply the algorithms
	apply_alg();
	
	cout << "****************END SCENARIO 2****************\n\n";
	/*----------scenario_2----------*/
	
	req_original = reqCP;	// reset to the original requests order
	
	/*----------scenario_3----------*/
	cout << "***************START SCENARIO 3***************\n";
	cout << "ADD 10 ITEM AT END OF QUEUE" << endl;
	
	req_original.insert(req_original.end(), new_req.begin(), new_req.end());
	
	visit_original.clear();
	
	for (uint i = 0; i < req_original.size(); ++i)
		visit_original.push_back(false);
	
	outputRequests();	// output the info of requests for scenario 3
	
	// apply the algorithms
	apply_alg();
	
	cout << "****************END SCENARIO 3****************\n\n";
	/*----------scenario_3----------*/
	
	req_original = reqCP;	// reset to the original requests order
	
	/*----------scenario_3----------*/
	cout << "***************START SCENARIO 4***************\n";
	cout << "ADD 10 ITEM AT RANDOM LOCATIONS OF QUEUE" << endl;
	
	for (uint i = 0; i < new_req.size(); ++i)
	{
		req_original.insert(req_original.begin() + randomInt(req_original.size()), new_req[i]);
	}
	
	visit_original.clear();
	
	for (uint i = 0; i < req_original.size(); ++i)
		visit_original.push_back(false);
	
	outputRequests();	// output the info of requests for scenario 3
	
	// apply the algorithms
	apply_alg();
	
	cout << "****************END SCENARIO 4****************\n\n";
	/*----------scenario_4----------*/
	
	req_original = reqCP;	// reset to the original requests order
	
	return 0;
}
