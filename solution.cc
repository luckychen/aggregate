#include <iostream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
using namespace std;

typedef struct _node{
	unsigned int duplicated; 
	unsigned int groupID;
	vector<string> data;
}node;

inline string extractSeq(string data){
	unsigned int firstL = data.find_first_not_of(' ');
	auto trimed = data.substr(firstL, data.size());
	unsigned int splitPo = trimed.find(' '); 
	return trimed.substr(0, splitPo - 1);
}


class arregatedData {
	private:
		unsigned int globalGroupCount;
		unsigned int globalUniqueCount;
		unsigned int dataLine;
		string fileName; 
		map<string, node*>& originData;
		vector<node*> firstHalfNodeVec;
		vector<node*> secondHalfNodeVec;	
		vector<map*> halfMapVec; 
		node *firstHalfNode, *secondHalfNode;
		map<string, node*> *secondHalfMap; 
		map<string, node*> *firstHalfMap;
		void buildHalfMap(originData);
	public:
	 
		void arregatedData(string fileName_):fileName(fileName_){
			globalGroupCount = 0;
			dataLine = 0;
			ifstream sampleRead("../all_sample.reads");
			//read the file and put every unique seq into a map
			if(sampleRead.open()){
				getline(sampleRead, line);//head line
				while(getline(sampleRead, line)){
					dataLine++;
					extractSeq(line, seq);		
					auto origIt = originData.find(seq);
					if(origIt != originData.end()){
						readData[seq]->data.push_back(line);
					} else {
						globalGroupCount +=1;
						temp = new node;
						dataPointBuf.push_back(temp);
						readData[seq] = dataPointBuf.back();	
					}
				}		
			} else {
				puts("file open failed");
			}
		}
		void arregatedExecution();
		void ~arregatedData(){
			for(auto const &i : firstHalfNodeVec){
				delete(*i);//all first half nodes were deleted 
			}
			for(auto const &i : secondHalfNodeVec){
				delete(*i);//all second half nodes were deleted
			}
			for(auto const &i : originData){
				delete(i->second);//all data nodes were deleted
			}
		}

}

void aggregatedData::buildHalfMap(){
	firstHalfMap = new map<string, node*>;
	secondHalfMap = new map<string, node*>;
	vector<string> dividedString;	
	for(auto &i : originData){
		dividedString = splitHalf(i.first);
		if(firstHalfMap.find(dividedString[0])){
			firstHalfMap[dividedString[0]]->duplicated += 1;
			firstHalfMap[dividedString[0]]->data.push_back[dividedString[1]];
		} else {
			firstHalfNode = new class node; 
			firstHalfNode->duplicated = 0;
			firstHalfNode->data.push_back(dividedString[1]);
			firstHalfMap[dividedString[0]] = firstHalfNode;
			firstHalfNodeVec.push_back(node);
		}
		if(secondHalfMap.find(dividedString[1])){
			secondHalfMap[dividedString[1]]->duplicated+=1;
			secondHalfMap[dividedString[1]]->data.push_back[dividedString[0]];
		} else {
			secondHalf = new class node;
			secondHalf->duplicated = 0;
			secondHalf->data.push_back(dividedString[0]);
			secondHalfMap[dividedString[1]] = secondHalf;
			secondHalfNodeVec.push_back(secondHalf);
		}
	}
}

void aggregatedData::arrangeGroupId(map<string, node*>* firstHalfMap, map<string, node*>* secondHalfMap, 
		const unsigned int len){

	vector<string> dividedString;
	for(auto &origIt : originData){
		dividedString = splitHalf(origIt.first);
		auto firstHalfKey = dividedString[0];
		auto secondHalfKey = dividedString[1];
		string origKey;
		auto it = firstHalfMap.find(firstHalf); 
		auto it2 = secondHalfMap.firstHalf(secondHalf);
		if(it != firstHalfMap.end()){
			for(auto &hitString : it->data){
				if(oneCharDiff(secondHalfKey, hitString) == true){
					hitKey = firstHalf.append(hitString);	
					if(origIt.groupID == 0 && originData[hitKey].groupID == 0){
						globalGroupCount += 1;
						originData[hitKey].groupID =  originData.groupID = globalGroupCount;
					} else {
						originData.groupID == 0 ? originData[hitKey].groupID = originData.groupID 
							: originData.groupID = originData[hitKey].groupID;
					}
				} 
			}		
		}
		if(it2 != secondHalfMap.end()){
			for(auto &hitString : it->data){
				if(oneCharDiff(firstHalfKey, hitString) == true){
					hitKey = hitString.append(secondHalfKey);	
					if(origIt.groupID == 0 && originData[hitKey].groupID == 0){
						globalGroupCount += 1;
						originData[hitKey].groupID =  originData.groupID = globalGroupCount;
					} else {
						originData.groupID == 0 ? originData[hitKey].groupID = originData.groupID 
							: originData.groupID = originData[hitKey].groupID;
					}
				} 
			}
		}

	}
}

void arregatedData::arregatedExecution(){
	for(unsigned int i = minSeqLen; i <= maxSeqLen; ++i){
		this->arrangeGroupId(halfMapVec[(i - minSeqLen)*2], halfMapVec[(i - minSeqLen)*2 + 1], i);
	}	
}


int main() {
	//read file
	string line;	
	string seq;
	map<string, node*> readData;
	vector<node*> dataPointBuf;
	node* temp;
	
	//find the similar seq with hamin distance less than 1
	class arregatedData(readData, minSeqLen, maxSeqLen);	
	arregatedData.store("all_samples_arregate");	

	for(auto &i : dataPointBuf){
		delete i;
	}

    return 0;
}
