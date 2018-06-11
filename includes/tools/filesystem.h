#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <cstdlib>

const std::string ROOT_PATH = "E:/MyOpenGL/";
const std::string TEXTURE_PATH = "resources/textures/";
const std::string OBJECT_PATH = "resources/objects/";

class FileSystem
{
private:
  typedef std::string (*Builder) (const std::string& path);

public:
  static std::string GetPath(const std::string& path)
  {
    static std::string(*pathBuilder)(std::string const &) = GetPathBuilder();
    return (*pathBuilder)(path);
  }

  static std::string GetTexturePath(const std::string& path)
  {
	  static std::string(*pathBuilder)(std::string const &) = GetPathBuilder();
	  return (*pathBuilder)(TEXTURE_PATH + path);
  }

  static std::string GetObjectPath(const std::string& path)
  {
	  static std::string(*pathBuilder)(std::string const &) = GetPathBuilder();
	  return (*pathBuilder)(OBJECT_PATH + path);
  }

private:
  static std::string const & GetRoot()
  {
    return ROOT_PATH;
  }
  static Builder GetPathBuilder()
  {
    if (GetRoot() != "")
      return &FileSystem::GetPathRelativeRoot;
    else
      return &FileSystem::GetPathRelativeBinary;
  }

  static std::string GetPathRelativeRoot(const std::string& path)
  {
    return GetRoot() + path;
  }

  static std::string GetPathRelativeBinary(const std::string& path)
  {
    return "../" + path;
  }


};

// FILESYSTEM_H
#endif
