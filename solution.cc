#include <iostream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <vector>
#include <list>
#include <unistd.h>
using namespace std;

typedef struct _node{
	unsigned int groupID;
	list<string> data;
	unsigned int readCount;
	string verifyString;
	string barCode;
}node;

inline bool nodeGreater (node* node1, node* node2)
{ return ( node1->readCount>=node2->readCount );}

typedef struct _mapS{
	unsigned int groupID;
	unsigned int readCount;
	bool operator<(const _mapS &rhs) const { return readCount< rhs.readCount; }
	bool operator>(const _mapS &rhs) const { return readCount> rhs.readCount; }
	bool operator==(const _mapS &rhs) const { return readCount== rhs.readCount; }
}mapS;



inline string extractSeq(string data){
	unsigned int firstL = data.find_first_not_of(' ');
	auto trimed = data.substr(firstL, data.size());
	unsigned int splitPo = min(trimed.find_first_of('\t'), trimed.find_first_of(' ')); 
	return trimed.substr(0, splitPo);
}

inline string extractCount(string data){
	unsigned int firstL = data.find_first_not_of(' ');
	auto trimed = data.substr(firstL, data.size());
	unsigned int splitPo;
	unsigned int i = 0;
	while(i < 4){
		splitPo = min(trimed.find_first_of('\t'), trimed.find_first_of(' ')); 
		trimed = trimed.substr(splitPo + 1, trimed.size());
		i++;
	}
	splitPo = min(trimed.find_first_of('\t'), trimed.find_first_of(' ')); 
	return trimed.substr(0, splitPo);
}


class aggregatedData{
	private:
		unsigned int globalGroupCount;
		unsigned int dataLine;
		bool V;
		string fileName; 
		string inputFileHead;
		map<string, node*> originData;
		node *firstHalfNode, *secondHalfNode;
		map<string, node*> secondHalfMap; 
		map<string, node*> firstHalfMap;
		vector<string> *aggregatedVec;
		vector<string> splitHalf(const string &input);
		vector<list<node*>> groupIdVec;
		vector<unsigned int> freeIdVec;
		unsigned int srinkGroupId();
		void updateGroup(string origKey, string hitKey);
		void moveIdVecElement(unsigned int id1, unsigned int id2);
		void buildHalfMap();
		bool oneCharDiff(string &S1, string &S2);
	public:
		aggregatedData(string fileName_, bool V_):fileName(fileName_), V(V_){
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
						originData[seq]->readCount += atoi(extractCount(line).c_str()); 
					}
					originData[seq]->data.push_back(line);
				} else {
					auto temp = new node;
					temp->groupID = 0;
					temp->barCode = seq;
					temp->data.push_back(line);
					temp->readCount = atoi(extractCount(line).c_str());
					originData[seq] = temp;	
				}
			}		
			groupIdVec.push_back({});
			buildHalfMap();
		}
		void writeResult(string file);
		void arrangeGroupId();
		void sortedInsert(list<node*>& listToUpdate, node* inputNode);
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

void aggregatedData::sortedInsert(list<node*>& listToUpdate, node* inputNode){
	list<node*>::iterator it;
	if(listToUpdate.size() == 0){
		listToUpdate.push_back(inputNode);
	} else {
		for(it=listToUpdate.begin(); it!=listToUpdate.end(); ++it){
			if(inputNode->readCount >= (*it)->readCount){
				listToUpdate.insert(it, inputNode);	
				break;
			}
		}
		if(it == listToUpdate.end())
			listToUpdate.push_back(inputNode);
	}
}

void aggregatedData::moveIdVecElement(unsigned int id1, unsigned int id2){
	//Move the element	
	if(groupIdVec[id1].size() == 0 || groupIdVec[id2].size() == 0){
		cout<<"error at list insert"<<endl;
		exit(0);
	} 
	for(auto it : groupIdVec[id2]){
		it->groupID = groupIdVec[id1].front()->groupID;
	}

	groupIdVec[id1].insert(groupIdVec[id1].end(), groupIdVec[id2].begin(), groupIdVec[id2].end());
	groupIdVec[id2].clear() ;
}

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

void aggregatedData::updateGroup(string origKey, string hitKey){
	unsigned int currentGroupId;
	unsigned origId, hitId;
	origId = originData[origKey]->groupID;
	hitId = originData[hitKey]->groupID;
	if(origId != 0 && hitId != 0  ){
		if(origId != hitId){
			moveIdVecElement(hitId, origId);	
			if(origId == groupIdVec.size()-1){
				groupIdVec.pop_back();
			} else {
				freeIdVec.push_back(origId);
			}
		}
	}else if(origId == 0 && hitId == 0){
		if(freeIdVec.size() > 0){
			currentGroupId = freeIdVec.back();
			freeIdVec.pop_back();
		} else {
			currentGroupId = groupIdVec.size();//new group
		}
		originData[hitKey]->groupID =  originData[origKey]->groupID = currentGroupId;
		if(currentGroupId == groupIdVec.size()){
			groupIdVec.push_back({});
		}
		groupIdVec[currentGroupId].push_back(originData[origKey]);
		groupIdVec[currentGroupId].push_back(originData[hitKey]);
		//sortedInsert(groupIdVec[currentGroupId], originData[hitKey]);
		originData[origKey]->verifyString = hitKey;
		originData[hitKey]->verifyString = origKey;
	} else {
		
		if(hitId == 0 )
		{	   
			originData[hitKey]->groupID = originData[origKey]->groupID;
			originData[hitKey]->verifyString = origKey;
			groupIdVec[originData[origKey]->groupID].push_back(originData[hitKey]);
			//sortedInsert(groupIdVec[originData[origKey]->groupID], originData[hitKey]);
		} else {
			originData[origKey]->groupID = originData[hitKey]->groupID;
			groupIdVec[originData[hitKey]->groupID].push_back(originData[origKey]);
			originData[origKey]->verifyString = hitKey;
			//sortedInsert(groupIdVec[originData[hitKey]->groupID], originData[origKey]);
		}
	}
}
void aggregatedData::arrangeGroupId(){

	vector<string> dividedString;
	unsigned int elementId = 0;
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
					if(hitKey < origKey)
					{
						updateGroup(origKey, hitKey);
					}
				} 
			}		
		}
		if(it2 != secondHalfMap.end()){
			for(auto &hitString : it2->second->data){
				if(oneCharDiff(firstHalfKey, hitString) == true){
					string hitKey = hitString;	
					hitKey.append(secondHalfKey);
					if(hitKey < origKey)
					{
						updateGroup(origKey, hitKey);
						
					}
				} 
			}
		}
		auto tempKey = origKey.substr(1, origKey.size()); 
		auto it3 = originData.find(tempKey);
		if(it3 != originData.end()){
			if(tempKey < origKey)
			{
				updateGroup(origKey, tempKey);
			}
		}
		
		tempKey = origKey.substr(0, origKey.size() - 1);
		auto it4 = originData.find(tempKey);
		if(it4 != originData.end()){
			if(tempKey < origKey)
			{
				updateGroup(origKey, tempKey);
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

unsigned int aggregatedData::srinkGroupId(){
	unsigned int lo = 0;//location in the vector	
	bool end = false;
	//vector<list<node*>>::iterator it;
	unsigned int origVecSize = groupIdVec.size();
	for(lo = 0; lo < origVecSize; ++lo){
		if(lo == groupIdVec.size() - 1)
			return lo;
		if(groupIdVec[lo].size() == 0 && lo > 0){
			while((groupIdVec.back()).size() == 0){
				//find the no-empty elements from end of list
				if(groupIdVec.size() == lo+1){
					//this branch means 
					//last iterator is the last no-empty iterator
					end = true;
					break;
				} else {
					groupIdVec.pop_back();	
				}
			}
			if(!end){
				groupIdVec[lo] = groupIdVec.back();
				for(auto &it2 : groupIdVec[lo]){
					it2->groupID = lo;
				}
				groupIdVec.pop_back();
			} else{
				//current iterator is back() and it is also a emplty list
				//lets pop current iterator and return
				groupIdVec.pop_back();
				return lo;
			}
		}
	}
}

void aggregatedData::writeResult(string outFileName){
	unsigned int realGroupCount = 0;
	realGroupCount = srinkGroupId();
	for(auto &it : groupIdVec){
		it.sort(nodeGreater);		
	}
	vector<list<node*>> groupIdVec_2;
	vector<mapS> mapSVec;
	vector<unsigned int> mapIntVec;
	mapSVec.resize(realGroupCount+1);
	mapIntVec.resize(realGroupCount+1);
	groupIdVec_2.resize(realGroupCount+1);
	unsigned int groupedNum = 0;
	unsigned int unGroupedNum = 0;
	for(auto &it : groupIdVec){
		groupedNum += it.size();
	}
	for(unsigned int i = 0; i <= realGroupCount; ++i){
		mapSVec[i].groupID = i;
	}	
	ofstream ofs(outFileName, ofstream::out);
	for(auto &it : originData){
		if(it.second->groupID != 0){
			mapSVec[(it.second)->groupID].readCount += (it.second)->readCount;
			//groupIdVec[(it.second)->groupID].push_back(it.second);
		} else {
			groupIdVec[0].push_back(it.second);	
			unGroupedNum += 1;
		}
	}
	sort(mapSVec.begin(), mapSVec.end(), greater<mapS>());

	for(unsigned int i = 0; i <= realGroupCount; ++i){
		mapIntVec[mapSVec[i].groupID] = i;
	}
	for(unsigned int id = 1; id <= realGroupCount; ++id){
		cout<<id<<endl;
		groupIdVec_2[mapIntVec[id]] = groupIdVec[id];
	}
	
	ofs << "groupID\t" + inputFileHead << endl;
	for(unsigned int i = 0; i < realGroupCount; ++i){
		for(auto &it : groupIdVec_2[i]){
			for(auto &it_j : it->data){
				if(V)
					ofs<<to_string(i + 1) + '\t' + it->verifyString + '\t' + it_j << endl;
				else
					ofs<<to_string(i + 1) + '\t' + it_j << endl;
			}
		}
	}
	unsigned int appendId = realGroupCount;
	for(auto &it : groupIdVec[0]){
		ofs<<to_string(appendId + 1) + '\t' + it->data.front() << endl;	
		appendId += 1;
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
	bool V = false;//verify
	while ((oc = getopt(argc, argv, "i:o:v:")) != -1) {	

		switch (oc) {
			case 'i':
				/* input data file name */
				sprintf(fileName, "%s", optarg);
				break;
			case 'o':
				/* output result file name */
				sprintf(resultName, "%s", optarg);
				break;
			case 'v':
				/* verification  */
				if(atoi(optarg) == 1)
					V = true;	
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

	class aggregatedData aggregatedData_(fileName, V);
	aggregatedData_.arrangeGroupId();	
	aggregatedData_.writeResult(resultName);

	return 0;
}
