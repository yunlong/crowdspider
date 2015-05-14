// by YunLong.Lee    <yunlong.lee@163.com> 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <sys/time.h>
using namespace std;

typedef string USERID;
const double EPS = 1e-9;

typedef struct {
	USERID uid;
	double rank;
} urank_t;

struct URankComparator
{  
	bool operator()(const urank_t& t1, const urank_t& t2)   
    {  
    	return  t1.rank > t2.rank;
    }  
};

class UserRank {
private:
	unsigned int 								totalDocNum;
	std::map<USERID, double> 					firstMapRank;
	std::map<USERID, std::map<USERID, double>* > 	matrix;
  	double 										alpha;
	double										beta;

private:
	std::map<USERID, int> mapUserIndegree;

public:
	UserRank( double d ) { alpha = d; }

	~UserRank()
	{
    	while(!matrix.empty())
    	{
        	std::map<USERID, std::map<USERID, double>* >::iterator xiter = matrix.begin();
       	 	if(xiter->second != NULL)
        	{
            	delete xiter->second;
            	matrix.erase(xiter);
        	}
    	}
	}

public:

	void addLink(USERID srcid, USERID dstid, double similarity)
	{	
		if(firstMapRank.find(srcid) == firstMapRank.end())
			firstMapRank[srcid] = 0.0;
		if(firstMapRank.find(dstid) == firstMapRank.end())
			firstMapRank[dstid] = 0.0;
		
		if(srcid == dstid) 
			return;
	
		std::map<USERID, double>* aMap = NULL;
		std::map<USERID, std::map<USERID, double>* >:: iterator xiter = matrix.find(srcid);
		if(xiter == matrix.end())
		{
			aMap = new std::map<USERID, double>;
			matrix[srcid] = aMap;	
		}
		else
		{
			aMap = xiter->second;
		}

		std::map<USERID, double>::iterator yiter = aMap->find(dstid);
		if(yiter == aMap->end())
		{
			(*aMap)[dstid] = similarity;
		}		
	}

	bool checkMatrixSelfLoop()
	{
		std::list<USERID> singleList;
		std::list<USERID> ustack;


	    std::map<USERID, std::map<USERID, double>* >::iterator xIter = matrix.begin();
  	    for( ; xIter != matrix.end(); xIter++ )
        {	
        	if(xIter->second != NULL && !xIter->second->empty())
			{
				if(xIter->second->size() == 1)
				{
					singleList.push_back(xIter->first);
				}
			}
    	}
	 

		std::list<USERID>::iterator uIt = singleList.begin();
		for( ; uIt != singleList.end(); uIt++ )
		{
	  		USERID curNode = *uIt;
		
			while(true)
			{
				if(matrix.find(curNode) == matrix.end())
				{
			    	ustack.clear();
					break;
				}
			
				if(matrix[curNode]->size() != 1 )
				{
					ustack.clear();
					break;
				}
				else
				{
					std::map<USERID, double>::iterator zIt = matrix[curNode]->begin();
					USERID next = zIt->first;

					bool exist = false;
					if( !ustack.empty() )
					{
						std::list<USERID>::iterator sit = ustack.begin();
						for( ; sit != ustack.end(); sit++ )
						{
							if(*sit == next)
							{
								exist = true;
								break;
							}
						}
					}

					if( exist )
					{
				//		std::list<USERID>::iterator sit = ustack.begin();
				//		for( ; sit != ustack.end(); sit++ )
				//		{
				//			cout << *sit << " ";
						//	matrix[*sit]->clear();
				//		}
				//	cout << " loop " << endl;
						matrix[curNode]->clear();
				    	ustack.clear();
						break;
					}
					else
					{	
						ustack.push_back(curNode);
						curNode = next;
					}
				}
			}
		}

	    std::map<USERID, std::map<USERID, double>* >::iterator yIter = matrix.begin();
  	    for( ; yIter != matrix.end(); yIter++ )
        {	
        	if(yIter->second != NULL && yIter->second->empty())
			{
            	delete yIter->second;
            	matrix.erase(yIter);
			}
    	}

	}

	bool buildMatrix()
	{
		ifstream ifs("jiwaifriends.txt");
   		string line;
    	while( getline(ifs, line) )
    	{
        	if( line.size() != 0 )
        	{
        		int pos1 = line.find_first_of(':');
				string srcid = line.substr(0, pos1);
				
        		std::map<USERID, int>::iterator iter = mapUserIndegree.find(srcid);
        		if( iter != mapUserIndegree.end())
                	mapUserIndegree[srcid]++;
        		else
                	mapUserIndegree[srcid] = 1;

				
        		int pos2 = line.find_first_of(' ', pos1 + 1);
			    while (pos2 != string::npos)
				{
        			string dstid = line.substr(pos1 + 1, pos2 - pos1 - 1 );
					//	cout << srcid << ':' << dstid << endl;
        			map<USERID, int>::iterator iter = mapUserIndegree.find(dstid);
        			if( iter != mapUserIndegree.end())
                		mapUserIndegree[dstid]++;
        			else
                		mapUserIndegree[dstid] = 1;

					addLink(srcid, dstid, 1.0);
					pos1 = pos2 ;	
					pos2 = line.find_first_of(' ', pos1 + 1);
				}
        	}
    	}
    	ifs.close();

		totalDocNum = firstMapRank.size();
		beta = 1.0 / totalDocNum;

		initMapRank(firstMapRank, beta);

		/***		
		std::map<USERID, double>::iterator fIter = firstMapRank.begin();
		for( ; fIter != firstMapRank.end(); fIter++ )
        	cout << fIter->first << endl;
	    std::map<USERID, std::map<USERID, double>* >::iterator xIter = matrix.begin();
  	    for( ; xIter != matrix.end(); xIter++ )
        {	
        	if(xIter->second != NULL && !xIter->second->empty())
        	{
            	std::map<USERID, double>::iterator yIter = xIter->second->begin();
            	for( ; yIter != xIter->second->end(); yIter++)
				{
					yIter->second = 1.0 / xIter->second->size();	
					cout << xIter->first << ':' << yIter->first << ':' << yIter->second << endl;
				}
        	}
    	}
		***/

    	return 0;
	}

	void showRanks(std::map<USERID, double>& ranks) 
	{
		std::map<USERID, double>::iterator Iter = ranks.begin();
		for( ; Iter != ranks.end(); Iter++ )
			cout << Iter->second << " "; 

		cout << endl;
		//	cout << Iter->first <<":" << Iter->second << endl; 
	}

	double sq(double x) { return x * x; }
	double diff(std::map<USERID,double>& prev, std::map<USERID, double>& next)
	{
    	double ret = 0.0;
		std::map<USERID, double>::iterator pIter = prev.begin();
		std::map<USERID, double>::iterator nIter = next.begin();
		for( ; nIter != next.end() && pIter != prev.end() ; nIter++, pIter++)
    	 	ret +=  sq( nIter->second - pIter->second ) ;
		cout << "diff val is : " << ret << endl;
    	return ret;
	}

	void initMapRank(std::map<USERID, double>& ranks, double val)
	{
		std::map<USERID, double>::iterator iter = ranks.begin();
		for( ; iter != ranks.end(); iter++ )
			iter->second = val;
	}
	
	bool computeUserRank(void)
	{
		std::map<USERID, double> allZeroMapRank;
		std::map<USERID, double>::iterator iter = firstMapRank.begin();
		for( ; iter != firstMapRank.end(); iter++ )
		{
			if( matrix.find(iter->first) == matrix.end() )
			{	
				allZeroMapRank[iter->first] = 1.0;
			}
		}

		std::map<USERID, double> prevRanks( firstMapRank.begin(), firstMapRank.end() );
		initMapRank(prevRanks, 0.0);
		std::map<USERID, double> curRanks( firstMapRank.begin(), firstMapRank.end() );

		int loopcnt = 0;
    	do 
		{
			loopcnt++;

      		curRanks.swap(prevRanks);
      		initMapRank(curRanks, 0.0);

	    	std::map<USERID, std::map<USERID, double>* >::iterator xIter = matrix.begin();
  	    	for( ; xIter != matrix.end(); xIter++ )
        	{
				/*	
				double linesum = 0.0;
            	std::map<USERID, double>::iterator sIter = xIter->second->begin();
            	for( ; sIter != xIter->second->end(); sIter++)
					linesum += sIter->second;
				*/
	
        		if(xIter->second != NULL)
        		{
                	const double val = 1.0 / xIter->second->size() * alpha;
            		std::map<USERID, double>::iterator yIter = xIter->second->begin();
            		for( ; yIter != xIter->second->end(); yIter++)
            		{	
						curRanks[ yIter->first ] += prevRanks[ xIter->first ] * val;//(yIter->second / linesum) * alpha;
            		}
        		}
    		}

			double sum = 0.0;
			std::map<USERID, double>::iterator zIter = allZeroMapRank.begin();
  	    	for( ; zIter != allZeroMapRank.end(); zIter++ )
				sum += prevRanks[zIter->first] * alpha * zIter->second;	

			double alphabeta = beta *  ( sum + (1 - alpha) );
			std::map<USERID, double>::iterator vIter = curRanks.begin();
  	    	for( ; vIter != curRanks.end(); vIter++ )
			{
				curRanks[ vIter->first ] += alphabeta; 
			}
		//	showRanks(prevRanks);
	   	} while ( diff(prevRanks, curRanks) > EPS );

		std::list<urank_t> uranklist;
		urank_t urank;

		double sumPR = 0.0;	
		std::map<USERID, double>::iterator cIter = curRanks.begin();
		for( ; cIter != curRanks.end(); cIter++ )
		{
        	//cout << cIter->first << "===>" << cIter->second << endl;
        	urank.uid = cIter->first;
			urank.rank = cIter->second;
			uranklist.push_back(urank);
			sumPR += cIter->second;
		}

		uranklist.sort(URankComparator());

		std::list<urank_t>::iterator uIter = uranklist.begin();
		for( ; uIter != uranklist.end(); uIter++ )
			cout << uIter->uid << "===>Rank: " << uIter->rank << "  indegree: " << mapUserIndegree[uIter->uid] - 1 << endl;

		cout << "sum of PageRank : "<< sumPR << '\t' << "loop count	: "<<  loopcnt << endl;

		return true;
  	}
};


int main() 
{
  	UserRank ur(0.85);
	ur.buildMatrix();
	ur.checkMatrixSelfLoop();

    timeval tv;
    gettimeofday( &tv, NULL );
    unsigned int start  = (tv.tv_sec*1000 + tv.tv_usec/1000);

	ur.computeUserRank();

    gettimeofday( &tv, NULL );
    unsigned int end  = (tv.tv_sec*1000 + tv.tv_usec/1000);
    cout << (end - start) / (double)1000 << endl;

  	return 0;
}

