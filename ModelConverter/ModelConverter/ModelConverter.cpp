// ModelConverter.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "structures/Blueprint.h"
#include "utils/InputStream.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	Blueprint blueprint;
	blueprint.loadModelInfo();
	blueprint.getComponents();
	blueprint.exportBlueprint();
}

