#include "optimization.h"

using namespace grid;

std::vector<float> readVertices3D(const std::string& path) {
  std::string line;
  std::ifstream is(path);
  std::vector<float> vertices;
  while(std::getline(is,line)) {
    char c;
    double vx,vy,vz;
    if((int)line.size()>1 && line[0]=='v' && line[1]==' ') {
      std::istringstream(line) >> c >> vx >> vy >> vz;
      vertices.push_back(vx);
      vertices.push_back(vy);
      vertices.push_back(vz);
    }
  }
  return vertices;
}
std::vector<int> readTriangles(const std::string& path) {
  std::string line;
  std::ifstream is(path);
  std::vector<int> faces;
  while(std::getline(is,line)) {
    char c;
    std::string fx,fy,fz;
    if((int)line.size()>1 && line[0]=='f' && line[1]==' ') {
      std::istringstream(line) >> c >> fx >> fy >> fz;
      faces.push_back(atoi(strtok((char*)fx.c_str(),"/"))-1);
      faces.push_back(atoi(strtok((char*)fy.c_str(),"/"))-1);
      faces.push_back(atoi(strtok((char*)fz.c_str(),"/"))-1);
    }
  }
  return faces;
}
int main() {
  std::string path="cube.obj";
  std::vector<float> pcoords=readVertices3D(path);
  std::vector<int> facevertices=readTriangles(path);
  setParameters(0,0,0,0,0,1,1e-3,128,1,0.3,2,false,false);

  setBoundaryCondition([](double pos[3])->bool {
    return pos[0]<0.1;
  },[](double pos[3])->bool {
    return pos[0]>0.9;
  },[](double pos[3])->Eigen::Matrix<double,3,1> {
    return Eigen::Matrix<double,3,1>(-1,0,0);
  });
  buildGrids(pcoords,facevertices);

  uploadTemplateMatrix();
  initDensities(0.3);
  update_stencil();

  for(int i=0; true; i++) {
    std::cout << "VCycle " << i << std::endl;
    grids.v_cycle();
  }
  return 0;
}
