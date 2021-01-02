// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Battle.proto

#ifndef PROTOBUF_Battle_2eproto__INCLUDED
#define PROTOBUF_Battle_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/map.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_Battle_2eproto();
void protobuf_AssignDesc_Battle_2eproto();
void protobuf_ShutdownFile_Battle_2eproto();

class GameCommond;
class GameCommondGroup;
class GamePlayerInfos;
class PlayerInfo;

// ===================================================================

class PlayerInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PlayerInfo) */ {
 public:
  PlayerInfo();
  virtual ~PlayerInfo();

  PlayerInfo(const PlayerInfo& from);

  inline PlayerInfo& operator=(const PlayerInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PlayerInfo& default_instance();

  void Swap(PlayerInfo* other);

  // implements Message ----------------------------------------------

  inline PlayerInfo* New() const { return New(NULL); }

  PlayerInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PlayerInfo& from);
  void MergeFrom(const PlayerInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(PlayerInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint32 id = 1;
  void clear_id();
  static const int kIdFieldNumber = 1;
  ::google::protobuf::uint32 id() const;
  void set_id(::google::protobuf::uint32 value);

  // optional uint32 actor_id = 2;
  void clear_actor_id();
  static const int kActorIdFieldNumber = 2;
  ::google::protobuf::uint32 actor_id() const;
  void set_actor_id(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:PlayerInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 actor_id_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static PlayerInfo* default_instance_;
};
// -------------------------------------------------------------------

class GamePlayerInfos : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:GamePlayerInfos) */ {
 public:
  GamePlayerInfos();
  virtual ~GamePlayerInfos();

  GamePlayerInfos(const GamePlayerInfos& from);

  inline GamePlayerInfos& operator=(const GamePlayerInfos& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GamePlayerInfos& default_instance();

  void Swap(GamePlayerInfos* other);

  // implements Message ----------------------------------------------

  inline GamePlayerInfos* New() const { return New(NULL); }

  GamePlayerInfos* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GamePlayerInfos& from);
  void MergeFrom(const GamePlayerInfos& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(GamePlayerInfos* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .PlayerInfo player_infos = 1;
  int player_infos_size() const;
  void clear_player_infos();
  static const int kPlayerInfosFieldNumber = 1;
  const ::PlayerInfo& player_infos(int index) const;
  ::PlayerInfo* mutable_player_infos(int index);
  ::PlayerInfo* add_player_infos();
  ::google::protobuf::RepeatedPtrField< ::PlayerInfo >*
      mutable_player_infos();
  const ::google::protobuf::RepeatedPtrField< ::PlayerInfo >&
      player_infos() const;

  // @@protoc_insertion_point(class_scope:GamePlayerInfos)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedPtrField< ::PlayerInfo > player_infos_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static GamePlayerInfos* default_instance_;
};
// -------------------------------------------------------------------

class GameCommond : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:GameCommond) */ {
 public:
  GameCommond();
  virtual ~GameCommond();

  GameCommond(const GameCommond& from);

  inline GameCommond& operator=(const GameCommond& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GameCommond& default_instance();

  void Swap(GameCommond* other);

  // implements Message ----------------------------------------------

  inline GameCommond* New() const { return New(NULL); }

  GameCommond* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GameCommond& from);
  void MergeFrom(const GameCommond& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(GameCommond* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional float x_axis = 1;
  void clear_x_axis();
  static const int kXAxisFieldNumber = 1;
  float x_axis() const;
  void set_x_axis(float value);

  // optional float y_axis = 2;
  void clear_y_axis();
  static const int kYAxisFieldNumber = 2;
  float y_axis() const;
  void set_y_axis(float value);

  // optional bool jump = 3;
  void clear_jump();
  static const int kJumpFieldNumber = 3;
  bool jump() const;
  void set_jump(bool value);

  // optional bool attack = 4;
  void clear_attack();
  static const int kAttackFieldNumber = 4;
  bool attack() const;
  void set_attack(bool value);

  // @@protoc_insertion_point(class_scope:GameCommond)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  float x_axis_;
  float y_axis_;
  bool jump_;
  bool attack_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static GameCommond* default_instance_;
};
// -------------------------------------------------------------------

class GameCommondGroup : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:GameCommondGroup) */ {
 public:
  GameCommondGroup();
  virtual ~GameCommondGroup();

  GameCommondGroup(const GameCommondGroup& from);

  inline GameCommondGroup& operator=(const GameCommondGroup& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GameCommondGroup& default_instance();

  void Swap(GameCommondGroup* other);

  // implements Message ----------------------------------------------

  inline GameCommondGroup* New() const { return New(NULL); }

  GameCommondGroup* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GameCommondGroup& from);
  void MergeFrom(const GameCommondGroup& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(GameCommondGroup* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  // map<uint32, .GameCommond> commonds = 1;
  int commonds_size() const;
  void clear_commonds();
  static const int kCommondsFieldNumber = 1;
  const ::google::protobuf::Map< ::google::protobuf::uint32, ::GameCommond >&
      commonds() const;
  ::google::protobuf::Map< ::google::protobuf::uint32, ::GameCommond >*
      mutable_commonds();

  // @@protoc_insertion_point(class_scope:GameCommondGroup)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  typedef ::google::protobuf::internal::MapEntryLite<
      ::google::protobuf::uint32, ::GameCommond,
      ::google::protobuf::internal::WireFormatLite::TYPE_UINT32,
      ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
      0 >
      GameCommondGroup_CommondsEntry;
  ::google::protobuf::internal::MapField<
      ::google::protobuf::uint32, ::GameCommond,
      ::google::protobuf::internal::WireFormatLite::TYPE_UINT32,
      ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
      0 > commonds_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static GameCommondGroup* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// PlayerInfo

// optional uint32 id = 1;
inline void PlayerInfo::clear_id() {
  id_ = 0u;
}
inline ::google::protobuf::uint32 PlayerInfo::id() const {
  // @@protoc_insertion_point(field_get:PlayerInfo.id)
  return id_;
}
inline void PlayerInfo::set_id(::google::protobuf::uint32 value) {
  
  id_ = value;
  // @@protoc_insertion_point(field_set:PlayerInfo.id)
}

// optional uint32 actor_id = 2;
inline void PlayerInfo::clear_actor_id() {
  actor_id_ = 0u;
}
inline ::google::protobuf::uint32 PlayerInfo::actor_id() const {
  // @@protoc_insertion_point(field_get:PlayerInfo.actor_id)
  return actor_id_;
}
inline void PlayerInfo::set_actor_id(::google::protobuf::uint32 value) {
  
  actor_id_ = value;
  // @@protoc_insertion_point(field_set:PlayerInfo.actor_id)
}

// -------------------------------------------------------------------

// GamePlayerInfos

// repeated .PlayerInfo player_infos = 1;
inline int GamePlayerInfos::player_infos_size() const {
  return player_infos_.size();
}
inline void GamePlayerInfos::clear_player_infos() {
  player_infos_.Clear();
}
inline const ::PlayerInfo& GamePlayerInfos::player_infos(int index) const {
  // @@protoc_insertion_point(field_get:GamePlayerInfos.player_infos)
  return player_infos_.Get(index);
}
inline ::PlayerInfo* GamePlayerInfos::mutable_player_infos(int index) {
  // @@protoc_insertion_point(field_mutable:GamePlayerInfos.player_infos)
  return player_infos_.Mutable(index);
}
inline ::PlayerInfo* GamePlayerInfos::add_player_infos() {
  // @@protoc_insertion_point(field_add:GamePlayerInfos.player_infos)
  return player_infos_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::PlayerInfo >*
GamePlayerInfos::mutable_player_infos() {
  // @@protoc_insertion_point(field_mutable_list:GamePlayerInfos.player_infos)
  return &player_infos_;
}
inline const ::google::protobuf::RepeatedPtrField< ::PlayerInfo >&
GamePlayerInfos::player_infos() const {
  // @@protoc_insertion_point(field_list:GamePlayerInfos.player_infos)
  return player_infos_;
}

// -------------------------------------------------------------------

// GameCommond

// optional float x_axis = 1;
inline void GameCommond::clear_x_axis() {
  x_axis_ = 0;
}
inline float GameCommond::x_axis() const {
  // @@protoc_insertion_point(field_get:GameCommond.x_axis)
  return x_axis_;
}
inline void GameCommond::set_x_axis(float value) {
  
  x_axis_ = value;
  // @@protoc_insertion_point(field_set:GameCommond.x_axis)
}

// optional float y_axis = 2;
inline void GameCommond::clear_y_axis() {
  y_axis_ = 0;
}
inline float GameCommond::y_axis() const {
  // @@protoc_insertion_point(field_get:GameCommond.y_axis)
  return y_axis_;
}
inline void GameCommond::set_y_axis(float value) {
  
  y_axis_ = value;
  // @@protoc_insertion_point(field_set:GameCommond.y_axis)
}

// optional bool jump = 3;
inline void GameCommond::clear_jump() {
  jump_ = false;
}
inline bool GameCommond::jump() const {
  // @@protoc_insertion_point(field_get:GameCommond.jump)
  return jump_;
}
inline void GameCommond::set_jump(bool value) {
  
  jump_ = value;
  // @@protoc_insertion_point(field_set:GameCommond.jump)
}

// optional bool attack = 4;
inline void GameCommond::clear_attack() {
  attack_ = false;
}
inline bool GameCommond::attack() const {
  // @@protoc_insertion_point(field_get:GameCommond.attack)
  return attack_;
}
inline void GameCommond::set_attack(bool value) {
  
  attack_ = value;
  // @@protoc_insertion_point(field_set:GameCommond.attack)
}

// -------------------------------------------------------------------

// GameCommondGroup

// map<uint32, .GameCommond> commonds = 1;
inline int GameCommondGroup::commonds_size() const {
  return commonds_.size();
}
inline void GameCommondGroup::clear_commonds() {
  commonds_.Clear();
}
inline const ::google::protobuf::Map< ::google::protobuf::uint32, ::GameCommond >&
GameCommondGroup::commonds() const {
  // @@protoc_insertion_point(field_map:GameCommondGroup.commonds)
  return commonds_.GetMap();
}
inline ::google::protobuf::Map< ::google::protobuf::uint32, ::GameCommond >*
GameCommondGroup::mutable_commonds() {
  // @@protoc_insertion_point(field_mutable_map:GameCommondGroup.commonds)
  return commonds_.MutableMap();
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Battle_2eproto__INCLUDED
