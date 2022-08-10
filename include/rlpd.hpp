#pragma once

#include <string>
#include <fstream>
#include <vector>

#include <iostream>

class entry_t {
public:
  entry_t();

  entry_t(const std::vector<int>& new_tag_indices, const std::vector<char>& new_image_data);

public:
  std::vector<int> tag_indices;

  unsigned int image_end_off;

  std::vector<char> image_data;
};

class PRNFile {
public:
  PRNFile();

  void write(const std::string& filepath);

  void read(const std::string& filepath);

  void add_tag(const std::string& tag);

  void add_entry(const entry_t& entry);

private:
  char magic[4];
  int image_count;

  std::vector<std::string> tag_pool;

private:
    std::vector<entry_t> entries;
};
