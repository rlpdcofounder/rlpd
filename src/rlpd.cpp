#include "../include/rlpd.hpp"

entry_t::entry_t() { image_end_off = 0; }

entry_t::entry_t(const std::vector<int> &new_tag_indices,
                 const std::vector<char> &new_image_data)
{
    tag_indices = new_tag_indices;
    image_data = new_image_data;

    image_end_off = tag_indices.size() * sizeof(int) + image_data.size();
}

PRNFile::PRNFile()
{
    magic[0] = 'R';
    magic[1] = 'L';
    magic[2] = 'P';
    magic[3] = 'D';
}

void PRNFile::write(const std::string &filepath)
{
    std::ofstream file(filepath, std::ios_base::binary);

    file.write(magic, 4);

    std::cout << "wrote magic: " << magic << std::endl;

    for (int i = 0; i < (int)tag_pool.size(); i++)
    {
        file.write(tag_pool[i].c_str(), tag_pool[i].size() + 1);

        std::cout << "wrote tag pool: " << tag_pool[i] << std::endl;
    }

    char seperator = 1;

    file.write(&seperator, 1);

    std::cout << "wrote seperator: "
              << "1" << std::endl;

    for (int i = 0; i < (int)entries.size(); i++)
    {
        std::cout << "begin entry" << std::endl;

        for (int j = 0; j < (int)entries[i].tag_indices.size(); j++)
        {
            file.write(
                reinterpret_cast<const char *>(&entries[i].tag_indices[j]),
                sizeof(int));

            std::cout << "wrote tag index: " << entries[i].tag_indices[j]
                      << std::endl;
        }

        file.write(&seperator, 1);

        std::cout << "wrote seperator: "
                  << "1" << std::endl;

        file.write(reinterpret_cast<const char *>(&entries[i].image_end_off),
                   sizeof(int));

        std::cout << "wrote image end offset: " << entries[i].image_end_off
                  << std::endl;

        file.write(entries[i].image_data.data(), entries[i].image_data.size());

        std::cout << "wrote image data: " << i << std::endl;
    }
}

void PRNFile::read(const std::string &filepath)
{
    char magic_bytes[4];

    std::ifstream file(filepath, std::ios_base::binary);

    file.read(magic_bytes, 4);

    for (int i = 0; i < 4; i++)
    {
        if (magic_bytes[i] != magic[i])
        {
            std::cerr << "we fucked up: wrong magic bytes" << std::endl;
        }
    }

    std::string current_tag = "";

    while (file.tellg() != 1)
    {
        if (file.tellg() == 0)
        {
            tag_pool.push_back(current_tag);
        }

        else
        {
            current_tag += file.tellg();
        }
    }

    entry_t new_entry;

    while (!file.eof())
    {
        while (file.peek() != 1)
        {
            int tag_index;

            file.read(reinterpret_cast<char *>(&tag_index), sizeof(int));

            new_entry.tag_indices.push_back(
                tag_index); // ur such a nerd but i admire it
        }

        file.read(reinterpret_cast<char *>(&new_entry.image_end_off),
                  sizeof(int));

        while (file.tellg() != new_entry.image_end_off + 1)
        {
            char new_byte;

            file.read(&new_byte, sizeof(char));

            new_entry.image_data.push_back(new_byte);
        }
    }
    // i feel like putting lengths in the file would speed up parsing it
}

void PRNFile::add_tag(const std::string &tag) { tag_pool.push_back(tag); }

void PRNFile::add_entry(const entry_t &entry) { entries.push_back(entry); }
