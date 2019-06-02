#include <fstream>
#include <iostream>
#include <string>

bool check_stream_for_string(const std::string &str, std::ifstream &stream)
{
    std::streampos begin = stream.tellg();

    for (std::size_t i = 0; i < str.size(); i++)
    {
        if (!stream.good())
            return false;

        char c = stream.get();

        if (c != str[i])
        {
            stream.seekg(begin);
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    std::string 
        ai_player_text = "48756d616e",
        human_player_text = "4149",
        patt_text = "3a20506174742067657365747a74",
        checkmated_text = "3a205363686163686d6174742067657365747a74",
        draw_text = "556e656e74736368696564656e";

    int 
        ai_player_checkmated = 0, 
        ai_player_patts = 0,
        human_player_checkmated = 0, 
        human_player_patts = 0,
        draws = 0;

    for (int i = 1; i < argc; i++)
    {
        std::ifstream stream(argv[i]);

        if (stream.fail())
        {
            std::cout 
                << "File \"" << argv[i] << "\" doesn't exist." << std::endl;
            stream.close();

            if (i == argc - 1)
                return EXIT_SUCCESS;
            else
                continue;
        }

        while (stream.good())
        {
            bool 
                ai = false,
                patt = false,
                draw = false;

            // Player type
            if (check_stream_for_string(ai_player_text, stream))
                ai = true;
            else if (check_stream_for_string(human_player_text, stream))
                ai = false;
            else if (check_stream_for_string(draw_text, stream))
                draw = true;
            else
            {
                stream.ignore();
                continue;
            }

            if (draw)
            {
                draws++;
                continue;
            }

            // Victory type
            while (stream.good())
            {
                if (check_stream_for_string(patt_text, stream))
                {
                    patt = true;
                    break;
                }
                else if (check_stream_for_string(checkmated_text, stream))
                {
                    patt = false;
                    break;
                }
                else
                {
                    stream.ignore();
                    continue;
                }
            }

            if (ai)
            {
                if (patt)
                    ai_player_patts++;
                else
                    ai_player_checkmated++;
            }
            else
            {
                if (patt)
                    human_player_patts++;
                else
                    human_player_checkmated++;
            }
        }

        stream.close();
    }

    std::cout 
        << "KI:" << "\n"
        << " Male Schachmatt gesetzt: " << ai_player_checkmated    << "\n"
        << " Male Patt gesetzt: "       << ai_player_patts         << "\n"
        << "Mensch:" << "\n"
        << " Male Schachmatt gesetzt: " << human_player_checkmated << "\n"
        << " Male Patt gesetzt: "       << human_player_patts      << "\n"
        << "Male Unentschieden: "      << draws                   << std::endl;

    return EXIT_SUCCESS;
}
