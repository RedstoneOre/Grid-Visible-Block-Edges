#include<lib/json.h>
#include<fstream>
#include<filesystem>
#include<cstdio>
#include<vector>
#include<string>

namespace fs{
	using namespace std::filesystem;
}

std::vector<fs::directory_entry> files;//The target files
std::string tmp_path;

//Convert the file
Json model,file_content;
//Convert condition format
Json ConvertFormat(std::string from){
	Json conditions = Json::array();
	//Convert
	from+=',';
	size_t start = 0;
	std::string tmp;size_t eqp/*position of `=`*/;
	for(size_t end = from.find(',');end < from.size();end=from.find(',',end+1)){
		tmp=from.substr(start,end-start);
		printf("Found condition %s(%d,%d)\n",tmp.data(),int(start),int(end));
		eqp=tmp.find('=');
		if(eqp != std::string::npos){
			conditions.push_back({{tmp.substr(0,eqp), tmp.substr(eqp+1)}});
		}
		start=end+1;
	}
	return Json {
		{"AND", conditions}
	};
};
//Convert
void Convert(std::string from,std::string to){
	//Read
	std::ifstream readStream(from.data());
	readStream >> file_content;
	readStream.close();
	//Convert
	bool variantsMode=0;
	if(!file_content.count("multipart")){
		file_content["multipart"]=Json::array();
		variantsMode=1;
	}
	Json* multipart = &file_content["multipart"];
	if(variantsMode){//`variants` mode
		Json variants = file_content["variants"];
		for(auto i = variants.begin();i != variants.end();++i){
			multipart->push_back(
				Json {
					{"apply", i.value()},
					{"when", ConvertFormat(i.key())}
				}
			);
		}
		file_content.erase("variants");
	}
	multipart->push_back(model);//Use the model
	//Write
	std::ofstream writeStream(to.data());
	writeStream << file_content.dump(2);
	writeStream.close();

	//Log
	printf("Converted from %s to %s\n",from.data(),to.data());
}

int main(){
	//Read The model
	std::ifstream modelStream("./model.json");
	modelStream >> model;
	modelStream.close();
	model=Json {
		{"when", {{"AND", Json::array()}}},
		{"apply", model}
	};
	printf("Model object:\n%s",model.dump(2).data());

	//Make result directory
	fs::create_directory("./result");
	//For each file in origin
	fs::directory_iterator origin("./origin");//iterator
	long long fileCount=0,nowCount=0;
	for(auto& i : origin){
		printf("Checking %lld %s...\n",fileCount+1,i.path().c_str());
		if(i.status().type() == fs::file_type::regular){
			//Check if it's a json file
			tmp_path=i.path().string();
			if(tmp_path.size() >= 5 && tmp_path.substr(tmp_path.size()-5)==".json"){
				//Pass the file
				files.push_back(i);
				++fileCount;
				printf("Pass %lld %s\n",fileCount,i.path().c_str());
			}
		}
	}
	for(auto& i : files){
		if(i.status().type() == fs::file_type::regular){
			++nowCount;
			printf("Converting %lld/%lld %s...\n",nowCount,fileCount,i.path().c_str());
			Convert(i.path().c_str(),i.path().string().replace(i.path().string().find("origin"),6,"result"));
		}
	}
	return 0;
}
