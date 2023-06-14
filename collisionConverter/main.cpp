#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "pugixml.hpp"

struct sbcData
{
    unsigned char m_type;
    float m_sizeX;
    float m_sizeY;
    float m_sizeZ;
    float m_posX;
    float m_posY;
    float m_posZ;
    float m_rotX;
    float m_rotY;
    float m_rotZ;
    float m_rotW;
    unsigned int m_boneID;
};
struct chainPartData
{
    unsigned int m_boneID;
    unsigned char m_type;
    float m_sizeX;
    float m_sizeY;
    float m_sizeZ;
    float m_offsetX;
    float m_offsetY;
    float m_offsetZ;
    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;
    float m_rotationW;
    float m_mass;
    float m_restutition;
    float m_friction;
    float m_linearDamping;
    float m_angularDamping;
    float m_lowerAngularLimitX;
    float m_lowerAngularLimitY;
    float m_lowerAngularLimitZ;
    float m_upperAngularLimitX;
    float m_upperAngularLimitY;
    float m_upperAngularLimitZ;
    float m_angularStiffnessX;
    float m_angularStiffnessY;
    float m_angularStiffnessZ;
    float m_lowerLinearLimitX;
    float m_lowerLinearLimitY;
    float m_lowerLinearLimitZ;
    float m_upperLinearLimitX;
    float m_upperLinearLimitY;
    float m_upperLinearLimitZ;
    float m_linearStiffnessX;
    float m_linearStiffnessY;
    float m_linearStiffnessZ;
};

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        std::string l_output(argv[1]);
        l_output.append(".bin");

        try
        {
            pugi::xml_document l_meta;
            pugi::xml_parse_result l_parseResult = l_meta.load_file(argv[1]);
            if(!l_parseResult) throw std::exception("XML parsing error");

            pugi::xml_node l_root = l_meta.child("collision");
            if(!l_root) throw std::exception("No 'collision' root");
            std::ofstream l_file(l_output, std::ios::out | std::ios::binary);
            if(l_file.fail()) throw std::exception("Unable to create binary file");

            unsigned char l_setter = 0xCBU;
            l_file.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));

            unsigned int l_sbcCount = 0U;
            pugi::xml_node l_statics = l_root.child("statics");
            if(l_statics)
            {
                std::cout << ("Parsing static bone collision ...") << std::endl;
                pugi::xml_attribute l_sbcCountAtr = l_statics.attribute("count");
                if(l_sbcCountAtr)
                {
                    l_sbcCount = l_sbcCountAtr.as_uint();
                    l_file.write(reinterpret_cast<char*>(&l_sbcCount), sizeof(unsigned int));

                    if(l_sbcCount > 0U)
                    {
                        for(pugi::xml_node l_sbcNode = l_statics.child("sbc"); l_sbcNode; l_sbcNode = l_sbcNode.next_sibling("sbc"))
                        {
                            if(l_sbcCount == 0U) throw std::exception("Count of 'sbc' nodes isn't equal to 'count' attribute");
                            sbcData l_sbcData;

                            pugi::xml_attribute l_attr = l_sbcNode.attribute("bone");
                            if(!l_attr) throw std::exception("One of 'sbc' nodes doesn't have 'bone' attribute");
                            l_sbcData.m_boneID = l_attr.as_uint();

                            l_attr = l_sbcNode.attribute("type");
                            if(!l_attr) throw std::exception("One of sbc nodes doesn't have 'type' attribute");
                            l_sbcData.m_type = static_cast<unsigned char>(l_attr.as_uint());

                            l_attr = l_sbcNode.attribute("offset");
                            if(!l_attr) throw std::exception("One of 'sbc' nodes doesn't have 'offset' attribute");
                            std::stringstream l_offsetStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_offsetStream >> l_sbcData.m_posX >> l_sbcData.m_posY >> l_sbcData.m_posZ;
                            if(l_offsetStream.fail()) throw std::exception("Invalid offset string");

                            l_attr = l_sbcNode.attribute("size");
                            if(!l_attr) throw std::exception("One of 'sbc' nodes doesn't have 'size' attribute");
                            std::stringstream l_sizeStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_sizeStream >> l_sbcData.m_sizeX >> l_sbcData.m_sizeY >> l_sbcData.m_sizeZ;
                            if(l_sizeStream.fail()) throw std::exception("Invalid size string");

                            l_attr = l_sbcNode.attribute("rotation");
                            if(!l_attr) throw std::exception("One of sbc nodes doesn't have 'rotation' attribute");
                            std::stringstream l_rotationStream(l_attr.as_string("0.0 0.0 0.0 1.0"));
                            l_rotationStream >> l_sbcData.m_rotX >> l_sbcData.m_rotY >> l_sbcData.m_rotZ >> l_sbcData.m_rotW;
                            if(l_rotationStream.fail()) throw std::exception("Invalid rotation string");

                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_type), sizeof(unsigned char));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_sizeX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_sizeY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_sizeZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_posX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_posY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_posZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotW), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_sbcData.m_boneID), sizeof(unsigned int));

                            l_sbcCount--;
                        }
                    }
                }
                else throw std::exception("No 'count' attribute at 'statics' subnode");
            }
            else
            {
                l_file.write(reinterpret_cast<char*>(&l_sbcCount), sizeof(unsigned int));
                std::cout << "No data of static bone collisions" << std::endl;
            }

            unsigned int l_chainsCount = 0U;
            pugi::xml_node l_chains = l_root.child("chains");
            if(l_chains)
            {
                std::cout << "Parsing dynamic bone chains ..." << std::endl;
                pugi::xml_attribute l_chainsCountAtr = l_chains.attribute("count");
                if(l_chainsCountAtr)
                {
                    l_chainsCount = l_chainsCountAtr.as_uint();
                    l_file.write(reinterpret_cast<char*>(&l_chainsCount), sizeof(unsigned int));

                    for(pugi::xml_node l_chain = l_chains.child("chain"); l_chain; l_chain = l_chain.next_sibling("chain"))
                    {
                        if(l_chainsCount == 0U) throw std::exception("Count of 'chain' nodes isn't equal to 'count' attribute");

                        pugi::xml_attribute l_attr = l_chain.attribute("count");
                        pugi::xml_attribute l_boneIDAtr = l_chain.attribute("bone");
                        if(!l_attr || !l_boneIDAtr) throw std::exception("No 'count' or 'bone' attribute at 'chain' node");

                        unsigned int l_chainPartsCount = l_attr.as_uint();
                        l_file.write(reinterpret_cast<char*>(&l_chainPartsCount), sizeof(unsigned int));
                        unsigned int l_chainBone = l_boneIDAtr.as_uint();
                        l_file.write(reinterpret_cast<char*>(&l_chainBone), sizeof(unsigned int));


                        for(pugi::xml_node l_part = l_chain.child("part"); l_part; l_part = l_part.next_sibling("part"))
                        {
                            if(l_chainPartsCount == 0U) throw std::exception("Count of 'part' nodes isn't equal to 'count' attribute");

                            chainPartData l_chainPartData;

                            l_attr = l_part.attribute("bone");
                            if(!l_attr) throw std::exception("No 'boneID' attribute at part node");
                            l_chainPartData.m_boneID = l_attr.as_int();

                            l_attr = l_part.attribute("type");
                            if(!l_attr) throw std::exception("No 'type' attribute at part node");
                            l_chainPartData.m_type = static_cast<unsigned char>(l_attr.as_uint());

                            l_attr = l_part.attribute("size");
                            if(!l_attr) throw std::exception("No 'size' attribute at part node");
                            std::stringstream l_sizeStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_sizeStream >> l_chainPartData.m_sizeX >> l_chainPartData.m_sizeY >> l_chainPartData.m_sizeZ;
                            if(l_sizeStream.fail()) throw std::exception("Invalid size string");

                            l_attr = l_part.attribute("offset");
                            if(!l_attr) throw std::exception("No 'offset' attribute at part node");
                            std::stringstream l_offsetStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_offsetStream >> l_chainPartData.m_offsetX >> l_chainPartData.m_offsetY >> l_chainPartData.m_offsetZ;
                            if(l_offsetStream.fail()) throw std::exception("Invalid offset string");

                            l_attr = l_part.attribute("rotation");
                            if(!l_attr) throw std::exception("No 'rotation' attribute at part node");
                            std::stringstream l_rotationStream(l_attr.as_string("0.0 0.0 0.0 1.0"));
                            l_rotationStream >> l_chainPartData.m_rotationX >> l_chainPartData.m_rotationY >> l_chainPartData.m_rotationZ >> l_chainPartData.m_rotationW;
                            if(l_rotationStream.fail()) throw std::exception("Invalid rotation string");

                            l_attr = l_part.attribute("mass");
                            if(!l_attr) throw std::exception("No 'mass' attribute at part node");
                            l_chainPartData.m_mass = l_attr.as_float(1.f);

                            l_attr = l_part.attribute("restutition");
                            if(!l_attr) throw std::exception("No 'restutition' attribute at part node");
                            l_chainPartData.m_restutition = l_attr.as_float(0.f);

                            l_attr = l_part.attribute("friction");
                            if(!l_attr) throw std::exception("No 'friction' attribute at part node");
                            l_chainPartData.m_friction = l_attr.as_float(0.5f);

                            l_attr = l_part.attribute("damping");
                            if(!l_attr) throw std::exception("No 'damping' attribute at part node");
                            std::stringstream l_dampingStream(l_attr.as_string("0.0 0.0"));
                            l_dampingStream >> l_chainPartData.m_linearDamping >> l_chainPartData.m_angularDamping;
                            if(l_dampingStream.fail()) throw std::exception("Invalid damping string");

                            l_attr = l_part.attribute("lowAngLim");
                            if(!l_attr) throw std::exception("No 'lowAngLim' attribute at part node");
                            std::stringstream l_lowAngLimStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_lowAngLimStream >> l_chainPartData.m_lowerAngularLimitX >> l_chainPartData.m_lowerAngularLimitY >> l_chainPartData.m_lowerAngularLimitZ;
                            if(l_lowAngLimStream.fail()) throw std::exception("Invalid low angle limit string");

                            l_attr = l_part.attribute("uppAngLim");
                            if(!l_attr) throw std::exception("No 'uppAngLim' attribute at part node");
                            std::stringstream l_uppAngLimStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_uppAngLimStream >> l_chainPartData.m_upperAngularLimitX >> l_chainPartData.m_upperAngularLimitY >> l_chainPartData.m_upperAngularLimitZ;
                            if(l_uppAngLimStream.fail()) throw std::exception("Invalid up angle limit string");

                            l_attr = l_part.attribute("angStiff");
                            if(!l_attr) throw std::exception("No 'angStiff' attribute at part node");
                            std::stringstream l_angStiffStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_angStiffStream >> l_chainPartData.m_angularStiffnessX >> l_chainPartData.m_angularStiffnessY >> l_chainPartData.m_angularStiffnessZ;
                            if(l_angStiffStream.fail()) throw std::exception("Invalid angle stiffness string");

                            l_attr = l_part.attribute("lowLinLim");
                            if(!l_attr) throw std::exception("No 'lowLinLim' attribute at part node");
                            std::stringstream l_lowLinLimStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_lowLinLimStream >> l_chainPartData.m_lowerLinearLimitX >> l_chainPartData.m_lowerLinearLimitY >> l_chainPartData.m_lowerLinearLimitZ;
                            if(l_lowLinLimStream.fail()) throw std::exception("Invalid low linear limit string");

                            l_attr = l_part.attribute("uppLinLim");
                            if(!l_attr) throw std::exception("No 'uppLinLim' attribute at part node");
                            std::stringstream l_uppLinLimStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_uppLinLimStream >> l_chainPartData.m_upperLinearLimitX >> l_chainPartData.m_upperLinearLimitY >> l_chainPartData.m_upperLinearLimitZ;
                            if(l_uppLinLimStream.fail()) throw std::exception("Invalid up linear limit string");

                            l_attr = l_part.attribute("linStiff");
                            if(!l_attr) throw std::exception("No 'linStiff' attribute at part node");
                            std::stringstream l_linStiffStream(l_attr.as_string("0.0 0.0 0.0"));
                            l_linStiffStream >> l_chainPartData.m_linearStiffnessX >> l_chainPartData.m_linearStiffnessY >> l_chainPartData.m_linearStiffnessZ;
                            if(l_linStiffStream.fail()) throw std::exception("Invalid linear stiffness string");

                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_boneID), sizeof(int));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_type), sizeof(unsigned char));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_sizeX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_sizeY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_sizeZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_offsetX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_offsetY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_offsetZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationW), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_mass), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_restutition), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_friction), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearDamping), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularDamping), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerAngularLimitX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerAngularLimitY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerAngularLimitZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperAngularLimitX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperAngularLimitY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperAngularLimitZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularStiffnessX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularStiffnessY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularStiffnessZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerLinearLimitX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerLinearLimitY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerLinearLimitZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperLinearLimitX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperLinearLimitY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperLinearLimitZ), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearStiffnessX), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearStiffnessY), sizeof(float));
                            l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearStiffnessZ), sizeof(float));

                            l_chainPartsCount--;
                        }
                        l_chainsCount--;
                    }
                }
                else throw std::exception("No 'count' or 'bone' attribute at 'chains' subnode");
            }
            else
            {
                l_file.write(reinterpret_cast<char*>(&l_chainsCount), sizeof(unsigned int));
                std::cout << "No data of bone chains" << std::endl;
            }
            l_file.flush();
            l_file.close();
            std::cout << "File converted" << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "Cleaning ..." << std::endl;
            std::remove(l_output.c_str());
        }
    }
    else std::cout << "Usage: [file path]" << std::endl;
    std::getchar();

    return EXIT_SUCCESS;
}
