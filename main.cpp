#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[]){
	int bus, entry,asso, power,missNum;
    string reg[60000];
    bool hit[60000];
	string str;
	ifstream ifCache(argv[1]);
	ifCache>>str>>str>>bus;
	ifCache>>str>>str>>entry;
	ifCache>>str>>str>>asso;
	ifCache.close();

    power=0;
    int tmp=entry;
    while(tmp/2 >0){
    	power++;
    	tmp/=2;
    }

	ifstream ifRef(argv[2]);
	int i=0;
	int len=0;
	string cache[entry][asso];
	bool isValid[entry][asso];
	for(i=0;i<entry;i++){
		for(int j=0;j<asso;j++)isValid[i][j]=false;
	}
	i=0;
	missNum=0;
	while(ifRef>>reg[i]){
		hit[i]=false;
		if(reg[i][0]=='0' || reg[i][0]=='1'){
			str=reg[i].substr(bus-power);
			int index=0;
			for(int j=0;j<power;j++){
				index*=2;
				index+=str[j]-'0';
			}

			//find
			for(int j=0;j<asso;j++){
				if(!isValid[index][j])break;
				if(cache[index][j]==reg[i].substr(0,bus-power)){
					hit[i]=true;
					str=cache[index][j];

					//used, drop to end
					int k;
					for(k=j+1;k<asso;k++){
						if(!isValid[index][k])break;
					    cache[index][k-1]=cache[index][k];
					}
					cache[index][k-1]=str;
					break;
				}
			}
			if(!hit[i]){
				missNum++;
				bool done=false;

				//set
				for(int j=0;j<asso;j++){
					if(isValid[index][j])continue;
					isValid[index][j]=true;
					cache[index][j]=reg[i].substr(0,bus-power);
					done=true;
				}
				//kick
				if(!done){
					for(int j=1;j<asso;j++){
						cache[index][j-1]=cache[index][j];
					}
					cache[index][asso-1]=reg[i].substr(0,bus-power);
				}
			}
			
		}
		i++;
		len++;		
	}
	ifRef.close();
	
	ofstream fout("index.rpt");
	fout<<"Student ID: 104060002"<<endl;
	fout<<"Addressing Bus: "<<bus<<endl;
	fout<<"Entries: "<<entry<<endl;
	fout<<"Associativity: "<<asso<<endl;
	fout<<"Indexing bits count: "<<power<<endl;
	fout<<"Indexing bits: ";
	for(int i=0;i<power;i++){
		fout<<bus-power+i<<" ";
	}
	fout<<endl;
	fout<<"Total cache miss: "<<missNum<<endl;
	fout<<endl;
	fout<<reg[0]<<" "<<reg[1]<<endl;
	for(i=2;i<len-1;i++){
		fout<<reg[i]<<" ";
		if(hit[i])fout<<"hit"<<endl;
		else fout<<"miss"<<endl;
	}
	fout<<reg[len-1];
	fout.close();

	return 0;
}