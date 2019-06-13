#include <iostream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <unistd.h>
using namespace std;

typedef struct _node{
	unsigned int groupID;
	vector<string> data;
}node;

inline string extractSeq(string data){
	unsigned int firstL = data.find_first_not_of(' ');
	auto trimed = data.substr(firstL, data.size());
	unsigned int splitPo = min(trimed.find_first_of('\t'), trimed.find_first_of(' ')); 
	return trimed.substr(0, splitPo);
}


class aggregatedData{
	private:
		unsigned int globalGroupCount;
		unsigned int dataLine;
		string fileName; 
		string inputFileHead;
		map<string, node*> originData;
		node *firstHalfNode, *secondHalfNode;
		map<string, node*> secondHalfMap; 
		map<string, node*> firstHalfMap;
		vector<string> *aggregatedVec;
		vector<string> splitHalf(const string &input);
		void buildHalfMap();
		bool oneCharDiff(string &S1, string &S2);
	public:
		aggregatedData(string fileName_):fileName(fileName_){
			globalGroupCount = 0;
			dataLine = 0;
			string line;
			ifstream sampleRead;
			//read the file and put every unique seq into a map
			sampleRead.open(fileName);
			getline(sampleRead, inputFileHead);//head line
			while(getline(sampleRead, line)){
				dataLine++;
				auto seq = extractSeq(line);		
				auto origIt = originData.find(seq);
				if(origIt != originData.end()){
					if(originData[seq]->groupID == 0){
						globalGroupCount += 1;
						originData[seq]->groupID = globalGroupCount;
					}
					originData[seq]->data.push_back(line);
				} else {
					auto temp = new node;
					temp->groupID = 0;
					temp->data.push_back(line);
					originData[seq] = temp;	
				}
			}		
			
			buildHalfMap();
		}
		void writeResult(string file);
		void arrangeGroupId();
		~aggregatedData(){
			for(auto const &i : originData){
				delete(i.second);//all data nodes were deleted
			}
			for(auto const &i : firstHalfMap){
				delete(i.second);//all data nodes were deleted
			}
			for(auto const &i : secondHalfMap){
				delete(i.second);//all data nodes were deleted
			}
		}

};

vector<string> aggregatedData::splitHalf(const string &input){
	vector<string> result(2);
	result[0] = input.substr(0, input.size()/2);
	result[1] = input.substr(input.size()/2, input.size() - input.size()/2);	
	return result;
}
void aggregatedData::buildHalfMap(){
	vector<string> dividedString;	
	for(auto &i : originData){
		dividedString = splitHalf(i.first);
		auto i1 = firstHalfMap.find(dividedString[0]);
		if(i1 != firstHalfMap.end()){
			firstHalfMap[dividedString[0]]->data.push_back(dividedString[1]);
		} else {
			firstHalfNode = new node; 
			firstHalfNode->data.push_back(dividedString[1]);
			firstHalfMap[dividedString[0]] = firstHalfNode;
		}
		auto i2 = secondHalfMap.find(dividedString[1]);
		if(i2 != secondHalfMap.end()){
			secondHalfMap[dividedString[1]]->data.push_back(dividedString[0]);
		} else {
			secondHalfNode = new node;
			secondHalfNode->data.push_back(dividedString[0]);
			secondHalfMap[dividedString[1]] = secondHalfNode;
		}
	}
}

void aggregatedData::arrangeGroupId(){

	vector<string> dividedString;
	for(auto &origIt : originData){
		dividedString = splitHalf(origIt.first);
		auto firstHalfKey = dividedString[0];
		auto secondHalfKey = dividedString[1];
		string origKey = origIt.first;
	
		auto it = firstHalfMap.find(firstHalfKey); 
		auto it2 = secondHalfMap.find(secondHalfKey);
		if(it != firstHalfMap.end()){
			for(auto &hitString : it->second->data){
				if(oneCharDiff(secondHalfKey, hitString) == true){
					string hitKey = firstHalfKey;	
					hitKey.append(hitString);
					if(origIt.second->groupID != 0 && originData[hitKey]->groupID != 0){
						continue;
					} else if(origIt.second->groupID == 0 && originData[hitKey]->groupID == 0){
						globalGroupCount += 1;
						originData[hitKey]->groupID =  originData[origKey]->groupID = globalGroupCount;
					} else {
						originData[hitKey]->groupID == 0 ? originData[hitKey]->groupID = originData[origKey]->groupID 
							: originData[origKey]->groupID = originData[hitKey]->groupID;
					}
				} 
			}		
		}
		if(it2 != secondHalfMap.end()){
			for(auto &hitString : it2->second->data){
				if(oneCharDiff(firstHalfKey, hitString) == true){
					string hitKey = hitString;	
					hitKey.append(secondHalfKey);
					if(origIt.second->groupID != 0 && originData[hitKey]->groupID != 0){
						continue;
					}else if(origIt.second->groupID == 0 && originData[hitKey]->groupID == 0){
						globalGroupCount += 1;
						originData[hitKey]->groupID = originData[origKey]->groupID = globalGroupCount;
					} else {
						originData[hitKey]->groupID == 0 ? originData[hitKey]->groupID = originData[origKey]->groupID 
							: originData[origKey]->groupID = originData[hitKey]->groupID;
					}
				} 
			}
		}
		auto tempKey = origKey.substr(1, origKey.size() - 1); 
		auto it3 = originData.find(tempKey);
		if(it3 != originData.end()){
			if(it3->second->groupID != 0 && origIt.second->groupID != 0){
				continue;
			}else if(it3->second->groupID == 0 && origIt.second->groupID == 0){
				globalGroupCount += 1;
				originData[tempKey]->groupID = originData[origKey]->groupID = globalGroupCount;
			} else {
				originData[tempKey]->groupID == 0 ? originData[tempKey]->groupID = originData[origKey]->groupID 
					: originData[origKey]->groupID = originData[tempKey]->groupID;
			}
		}
		
		tempKey = origKey.substr(0, origKey.size() - 1);
		auto it4 = originData.find(tempKey);
		if(it4 != originData.end()){
			if(it4->second->groupID != 0 && origIt.second->groupID != 0){
				continue;
			} else if(it4->second->groupID == 0 && origIt.second->groupID == 0){
				globalGroupCount += 1;
				originData[tempKey]->groupID = originData[origKey]->groupID = globalGroupCount;
			} else {
				originData[tempKey]->groupID == 0 ? originData[tempKey]->groupID = originData[origKey]->groupID 
					: originData[origKey]->groupID = originData[tempKey]->groupID;
			}
		}

	}
}

bool aggregatedData::oneCharDiff(string& s1, string& s2){
	unsigned int diff;
	if(s1.length() != s2.length())
		return false;
	diff = 0;
	for(unsigned int i = 0; i < s1.length(); ++i){
		if(s1.at(i) != s2.at(i)){
			diff += 1;
		}
		if(diff > 1){
			return false;
		}
	}
	if(diff == 1)
		return true;

	return false;
}
void aggregatedData::writeResult(string outFileName){
	vector<vector<node*>> groupIdVec;
	for(unsigned int i = 0; i <= globalGroupCount; ++i){
		groupIdVec.push_back({});
	}	
	ofstream ofs(outFileName, ofstream::out);
	for(auto &it : originData){
		if(it.second->groupID != 0){
			groupIdVec[(it.second)->groupID].push_back(it.second);	
		}
	}
	ofs << "groupID\t" + inputFileHead << endl;
	for(unsigned int i = 0; i < groupIdVec.size(); ++i){
		for(auto &it : groupIdVec[i]){
			for(unsigned int j = 0; j < it->data.size(); ++j){
				ofs<<to_string(i) + '\t' + it->data[j] << endl;
			}
		}
	}
	ofs.close();
}


int main(int argc, char* argv[]) {
	//read file
	string line;	
	string seq;
	map<string, node*> readData;
	node* temp;
	char fileName[100];
	char resultName[100];
	int oc;
	while ((oc = getopt(argc, argv, "i:o:")) != -1) {	

		switch (oc) {
			case 'i':
				/* input data file name */
				sprintf(fileName, "%s", optarg);
				break;
			case 'o':
				/* output result file name */
				sprintf(resultName, "%s", optarg);
				break;
			case ':':
				printf("missing arguments\n");
				exit(0);
				break;
			case '?':
				printf("unrecongnized option\n");
				break;
			default:
				printf("option/arguments error!\n");       /* error handling, see text */
				exit(0);

		}
	}
	if (fileName[0] == '\0' || resultName[0] == '\0'){
		cout<<"input file name or reslut file name missing"<<endl;
		cout<<"execute it as ./test -i [input file name] -o [output file name]"<<endl;
		exit(0);
	}

	class aggregatedData aggregatedData_(fileName);
	aggregatedData_.arrangeGroupId();	
	aggregatedData_.writeResult(resultName);

	return 0;
}
