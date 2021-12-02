#pragma once
#include <string>

class Dataloader
{
public:
  std::string dataset_name;
  std::string dataset_path;

  Dataloader(std::string foobar);

  // Setters
  void set_dataset_name(std::string dataset_name_in);
  void set_dataset_path(std::string path_in);

  // Getters
  std::string get_dataset_name();
  std::string get_dataset_path();
};