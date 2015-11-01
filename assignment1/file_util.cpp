#include "file_util.hpp"

void load_model(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors){
    std::ifstream modelFile(fileName.c_str());

    while(1){
	float v1, v2, v3, c1, c2, c3,c4;

	modelFile >> v1;
	modelFile >> v2;
	modelFile >> v3;

	modelFile >> c1;
	modelFile >> c2;
	modelFile >> c3;
	modelFile >> c4;

	if(modelFile.eof()) {
	    break;
	}

	glm::vec4 newPosition(v1, v2, v3, 1.0);
	glm::vec4 newColor(c1, c2, c3, c4);

	positions.push_back(newPosition);
	colors.push_back(newColor);
   }
}

void save_model(std::string fileName, std::vector<glm::vec4> &positions, std::vector<glm::vec4> &colors){
    std:: ofstream modelFile( (fileName).c_str(), std::ios_base::out | std::ios_base::trunc);

    for(int i=0; i<positions.size(); i++){
	modelFile << positions[i][0] <<"      "<<positions[i][1] <<"	   " <<positions[i][2]<< "      ";
	modelFile << colors[i][0]    <<"      "<<colors[i][1] << "      " << colors[i][2] <<"       "<<colors[i][3]<< std::endl;
    }			

    modelFile.close();

}
