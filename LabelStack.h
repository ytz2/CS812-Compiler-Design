/*
 * LabelStack.h
 *
 *  Created on: Feb 27, 2013
 *      Author: yanhualiu
 */

#ifndef LABELSTACK_H_
#define LABELSTACK_H_

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

class LabelStack
{
public:
	LabelStack(string name)
	{
	  local_name=name;
	  count=0;
	}
	void push()
	{
	  stringstream convert;
	  convert<<local_name<<count;
	  count++;
	  Stack.push(convert.str());
	}
	void pop()
	{
	  Stack.pop();
	}
	bool isEmpty()
	{
	  return Stack.empty();
	}
	string top()
	{
	  return Stack.top();
	}
	~LabelStack()
	  {
	    while(!Stack.empty())
	      Stack.pop();
	  }
private:
	string local_name;
	long count;
	stack<string> Stack;
};


#endif /* LABELSTACK_H_ */
