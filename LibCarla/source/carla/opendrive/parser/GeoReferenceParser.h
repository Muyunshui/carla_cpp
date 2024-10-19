// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once
/// @brief һ���ṩXML�ĵ����࣬���ڴ���ͽ���XML����
namespace pugi {
  class xml_document;
} // namespace pugi
/// @brief Carla�Զ���ʻ�����ܵ������ռ�
namespace carla {
/// @brief ������·��ģ�͵�ͼ������ع��ܵ������ռ�
namespace road {
  class MapBuilder;/// @brief ���ڹ�����·��ͼ���࣬�ṩ��ͼ�����Ľӿںͷ���
} // namespace road
/// @brief ����OpenDrive��ʽ��·���ݽ����ʹ����ܵ������ռ�
namespace opendrive {
 /// @brief �������ڽ���OpenDrive�ļ��Ľ�������������ռ�
namespace parser {
 /// @brief ���ڽ���OpenDrive�ļ��еĵ���ο���Ϣ�Ľ�������
  class GeoReferenceParser {
  public:
/// @brief ����XML�ĵ��еĵ���ο���Ϣ��������·��ͼ
/// �ú�����ȡXML�ĵ��еĵ���ο����ݣ�������ϵͳ��ͶӰ��Ϣ
/// ��ʹ����Щ��Ϣ����������µ�·��ͼ
/// @param xml [const pugi::xml_document &] Ҫ������XML�ĵ�����
/// @param map_builder [carla::road::MapBuilder &] ���ڹ�����·��ͼ�Ĺ���������
    static void Parse(
        const pugi::xml_document &xml,
        carla::road::MapBuilder &map_builder);
  };

} // namespace parser
} // namespace opendrive
} // namespace carla
