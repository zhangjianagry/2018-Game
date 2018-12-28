#include "Blueprint.h"
#include "../utils/InputStream.h"
#include "../utils/OBJLoader.h"
Blueprint::Blueprint()
{
}


Blueprint::~Blueprint()
{
}

void Blueprint::loadModelInfo()
{
	overWater = InputStream::getBool("bulePrint ��overWater����:");
	underWater = InputStream::getBool("bulePrint ��underWater����:");
	do
	{
		SubBlueprint* subBlueprint = OBJLoader::loadOBJFiles(&materialsCache);
		if (subBlueprint != NULL)
		{
			subBlueprints.push_back(subBlueprint);
		}
	} while (InputStream::getBool("�Ƿ��������obj�ļ������ص�obj�ĸ���Ϊ" + std::to_string(subBlueprints.size())));

	if (InputStream::getBool("�Ƿ�Ҫ�޸�material�ļ�"))
	{
		map<string, Material>::iterator iter;
		for (iter = materialsCache.begin(); iter != materialsCache.end(); iter++) {
			cout << iter->first << " : " << iter->second.colour.toString() << endl;
		}
		while (true)
		{
			string name = InputStream::getString("�������޸ĵ�material������");
			try
			{
				materialsCache.at(name).colour.SetFlag(-1);
				string rgb = InputStream::getString("����material��rgb��ʽ");
				materialsCache.at(name).colour.SetCode(rgbToFloat(rgb));
				materialsCache.at(name).colour.SetOffset(InputStream::getFloat("����material��offset"));
				isCostomMaterial = true;
			}
			catch (const std::exception&)
			{
				cout << "�����ļ���������������" << endl;
			}
			if (!InputStream::getBool("�Ƿ�����޸�"))
			{
				break;
			}
		}
	}
	if (isCostomMaterial)
	{
		baseColour1.setX(InputStream::getFloat("����base��ɫ1��x"));
		baseColour1.setY(InputStream::getFloat("����base��ɫ1��y"));
		baseColour1.setZ(InputStream::getFloat("����base��ɫ1��z"));
		if (InputStream::getBool("�Ƿ�����base��ɫ2"))
		{
			baseColour2.setX(InputStream::getFloat("����base��ɫ1��x"));
			baseColour2.setY(InputStream::getFloat("����base��ɫ1��y"));
			baseColour2.setZ(InputStream::getFloat("����base��ɫ1��z"));
		}
		else {
			baseColour2 = baseColour1;
		}
	}
}

void Blueprint::getComponents()
{

	while (InputStream::getBool("�Ƿ��������component�ļ���Ŀǰ�Ѽ��ص�obj�ĸ���Ϊ" + std::to_string(components.size())))
	{
		map<int, string>::iterator iter;
		for (iter = ComponentFactor::componentsMap.begin(); iter != ComponentFactor::componentsMap.end(); iter++) {
			cout << iter->first << " : " << iter->second << endl;
		}
		cout << "-1" << " : " << "�˳�" << endl;
		Component* component = ComponentFactor::createComponent(InputStream::getInt("������Ҫ��ӵ�component�ı��(����)"));
		if (component != nullptr)
		{
			components.push_back(component);
		}
	} 
}

void Blueprint::exportBlueprint()
{
	string filename = InputStream::getString("����������ļ����ļ���(������.txt��׺)");
	ofstream outfile("out/" +  filename + ".txt", fstream::out);
	if (outfile)
	{
		outfile << "underwater :" << this->underWater << "	" << "overwater :" << this->overWater << endl;
		outfile << "subBlueprint������Ϊ��" << this->subBlueprints.size() << endl;
		for (int i = 0; i < subBlueprints.size(); i++)
		{
			outfile << "��" << i << "��subBlueprint" << endl;
			outfile << "	" << "subBlueprint�Ķ���ĸ���Ϊ:" << subBlueprints[i]->getVertexCount() << endl;
			outfile << "	" << "subBlueprint��positions���У�����position����Ϊ:"
				<< "x:" << subBlueprints[i]->getMax().getX() << " "
				<< "y:" << subBlueprints[i]->getMax().getY() << " "
				<< "z:" << subBlueprints[i]->getMax().getZ() << " " << endl;
			outfile << "	" << "subBlueprint��positions���У���С��position����Ϊ:"
				<< "x:" << subBlueprints[i]->getMin().getX() << " "
				<< "y:" << subBlueprints[i]->getMin().getY() << " "
				<< "z:" << subBlueprints[i]->getMin().getZ() << " " << endl;
			outfile << "	" << "sections������Ϊ��" << subBlueprints[i]->getSections().size() << endl;
			for (int j = 0; j < subBlueprints[i]->getSections().size(); j++)
			{
				outfile <<  "	" << "��" << j << "��section" << endl;
				outfile << "	" << "		" << "����ĸ���Ϊ��" << (subBlueprints[i]->getSections())[j]->getVertexCount() << endl;
				outfile << "	" << "		" << "section���ʵı��룺" << (subBlueprints[i]->getSections())[j]->getMaterial()->colour.toString() << endl;
				vector<Triangle> triangles = (subBlueprints[i]->getSections())[j]->getTriangles();
				for (int k = 0; k < triangles.size(); k++)
				{
					outfile << "	" << "		" << "��" << k << "��������" << endl;
					outfile << "	" << "		" <<  "		" << triangles[k].toString() << endl;
				}
			}
		}

		outfile << endl;
		outfile << endl;

		if (isCostomMaterial)
		{
			outfile << "component��baseColour1: " << baseColour1.getX() << "," << baseColour1.getY() << "," <<baseColour1.getZ() << endl;
			outfile << "component��baseColour2: " << baseColour2.getX() << "," << baseColour2.getY() << "," << baseColour2.getZ() << endl;
		}
		outfile << "component������Ϊ��" << this->components.size() << endl;
		for (int i = 0; i < components.size(); i++)
		{
			outfile << "��" << i << "��component" << endl;
			outfile << components[i]->toString() << endl;
		}
		outfile.close();
	}
}

int Blueprint::rgbToFloat(string rgb)
{
	float code = 0;
	bool r = false, g = false, b = false;
	for (int i = 0; i < rgb.size(); i++)
	{
		if (rgb[i] == 'r' || rgb[i] == 'R')
		{
			r = true;
		}
		else if (rgb[i] == 'g' || rgb[i] == 'G') {
			g = true;
		}
		else if (rgb[i] == 'b' || rgb[i] == 'B') {
			b = true;
		}
	}
	if (r == true) {
		code += 1;
	}
	if (g == true)
	{
		code += 2;
	}
	if (b == true) 
	{
		code += 4;
	}
	
	return code;
}




