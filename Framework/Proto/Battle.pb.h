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

namespace Proto {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_Battle_2eproto();
void protobuf_AssignDesc_Battle_2eproto();
void protobuf_ShutdownFile_Battle_2eproto();

class CS2CPlusPlusCommand;
class GameCommand;
class GameCommandGroup;
class GamePlayerInfos;
class Ping;
class PlayerInfo;
class StartBattleInfo;

// ===================================================================

class PlayerInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Proto.PlayerInfo) */ {
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

  // optional string actor_asset = 2;
  void clear_actor_asset();
  static const int kActorAssetFieldNumber = 2;
  const ::std::string& actor_asset() const;
  void set_actor_asset(const ::std::string& value);
  void set_actor_asset(const char* value);
  void set_actor_asset(const char* value, size_t size);
  ::std::string* mutable_actor_asset();
  ::std::string* release_actor_asset();
  void set_allocated_actor_asset(::std::string* actor_asset);

  // @@protoc_insertion_point(class_scope:Proto.PlayerInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr actor_asset_;
  ::google::protobuf::uint32 id_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static PlayerInfo* default_instance_;
};
// -------------------------------------------------------------------

class GamePlayerInfos : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Proto.GamePlayerInfos) */ {
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

  // repeated .Proto.PlayerInfo player_infos = 1;
  int player_infos_size() const;
  void clear_player_infos();
  static const int kPlayerInfosFieldNumber = 1;
  const ::Proto::PlayerInfo& player_infos(int index) const;
  ::Proto::PlayerInfo* mutable_player_infos(int index);
  ::Proto::PlayerInfo* add_player_infos();
  ::google::protobuf::RepeatedPtrField< ::Proto::PlayerInfo >*
      mutable_player_infos();
  const ::google::protobuf::RepeatedPtrField< ::Proto::PlayerInfo >&
      player_infos() const;

  // @@protoc_insertion_point(class_scope:Proto.GamePlayerInfos)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedPtrField< ::Proto::PlayerInfo > player_infos_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static GamePlayerInfos* default_instance_;
};
// -------------------------------------------------------------------

class StartBattleInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Proto.StartBattleInfo) */ {
 public:
  StartBattleInfo();
  virtual ~StartBattleInfo();

  StartBattleInfo(const StartBattleInfo& from);

  inline StartBattleInfo& operator=(const StartBattleInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const StartBattleInfo& default_instance();

  void Swap(StartBattleInfo* other);

  // implements Message ----------------------------------------------

  inline StartBattleInfo* New() const { return New(NULL); }

  StartBattleInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const StartBattleInfo& from);
  void MergeFrom(const StartBattleInfo& from);
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
  void InternalSwap(StartBattleInfo* other);
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

  // optional uint32 my_id = 1;
  void clear_my_id();
  static const int kMyIdFieldNumber = 1;
  ::google::protobuf::uint32 my_id() const;
  void set_my_id(::google::protobuf::uint32 value);

  // optional .Proto.GamePlayerInfos player_infos = 2;
  bool has_player_infos() const;
  void clear_player_infos();
  static const int kPlayerInfosFieldNumber = 2;
  const ::Proto::GamePlayerInfos& player_infos() const;
  ::Proto::GamePlayerInfos* mutable_player_infos();
  ::Proto::GamePlayerInfos* release_player_infos();
  void set_allocated_player_infos(::Proto::GamePlayerInfos* player_infos);

  // optional uint32 start_time = 3;
  void clear_start_time();
  static const int kStartTimeFieldNumber = 3;
  ::google::protobuf::uint32 start_time() const;
  void set_start_time(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:Proto.StartBattleInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::Proto::GamePlayerInfos* player_infos_;
  ::google::protobuf::uint32 my_id_;
  ::google::protobuf::uint32 start_time_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static StartBattleInfo* default_instance_;
};
// -------------------------------------------------------------------

class CS2CPlusPlusCommand : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Proto.CS2CPlusPlusCommand) */ {
 public:
  CS2CPlusPlusCommand();
  virtual ~CS2CPlusPlusCommand();

  CS2CPlusPlusCommand(const CS2CPlusPlusCommand& from);

  inline CS2CPlusPlusCommand& operator=(const CS2CPlusPlusCommand& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CS2CPlusPlusCommand& default_instance();

  void Swap(CS2CPlusPlusCommand* other);

  // implements Message ----------------------------------------------

  inline CS2CPlusPlusCommand* New() const { return New(NULL); }

  CS2CPlusPlusCommand* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CS2CPlusPlusCommand& from);
  void MergeFrom(const CS2CPlusPlusCommand& from);
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
  void InternalSwap(CS2CPlusPlusCommand* other);
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

  // optional int32 skill = 4;
  void clear_skill();
  static const int kSkillFieldNumber = 4;
  ::google::protobuf::int32 skill() const;
  void set_skill(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Proto.CS2CPlusPlusCommand)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  float x_axis_;
  float y_axis_;
  bool jump_;
  ::google::protobuf::int32 skill_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static CS2CPlusPlusCommand* default_instance_;
};
// -------------------------------------------------------------------

class GameCommand : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Proto.GameCommand) */ {
 public:
  GameCommand();
  virtual ~GameCommand();

  GameCommand(const GameCommand& from);

  inline GameCommand& operator=(const GameCommand& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GameCommand& default_instance();

  void Swap(GameCommand* other);

  // implements Message ----------------------------------------------

  inline GameCommand* New() const { return New(NULL); }

  GameCommand* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GameCommand& from);
  void MergeFrom(const GameCommand& from);
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
  void InternalSwap(GameCommand* other);
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

  // optional uint64 x_axis = 1;
  void clear_x_axis();
  static const int kXAxisFieldNumber = 1;
  ::google::protobuf::uint64 x_axis() const;
  void set_x_axis(::google::protobuf::uint64 value);

  // optional uint64 y_axis = 2;
  void clear_y_axis();
  static const int kYAxisFieldNumber = 2;
  ::google::protobuf::uint64 y_axis() const;
  void set_y_axis(::google::protobuf::uint64 value);

  // optional bool jump = 3;
  void clear_jump();
  static const int kJumpFieldNumber = 3;
  bool jump() const;
  void set_jump(bool value);

  // optional int32 skill = 4;
  void clear_skill();
  static const int kSkillFieldNumber = 4;
  ::google::protobuf::int32 skill() const;
  void set_skill(::google::protobuf::int32 value);

  // optional uint32 frame = 5;
  void clear_frame();
  static const int kFrameFieldNumber = 5;
  ::google::protobuf::uint32 frame() const;
  void set_frame(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:Proto.GameCommand)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::uint64 x_axis_;
  ::google::protobuf::uint64 y_axis_;
  bool jump_;
  ::google::protobuf::int32 skill_;
  ::google::protobuf::uint32 frame_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static GameCommand* default_instance_;
};
// -------------------------------------------------------------------

class GameCommandGroup : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Proto.GameCommandGroup) */ {
 public:
  GameCommandGroup();
  virtual ~GameCommandGroup();

  GameCommandGroup(const GameCommandGroup& from);

  inline GameCommandGroup& operator=(const GameCommandGroup& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GameCommandGroup& default_instance();

  void Swap(GameCommandGroup* other);

  // implements Message ----------------------------------------------

  inline GameCommandGroup* New() const { return New(NULL); }

  GameCommandGroup* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GameCommandGroup& from);
  void MergeFrom(const GameCommandGroup& from);
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
  void InternalSwap(GameCommandGroup* other);
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

  // optional uint32 frame = 1;
  void clear_frame();
  static const int kFrameFieldNumber = 1;
  ::google::protobuf::uint32 frame() const;
  void set_frame(::google::protobuf::uint32 value);

  // map<uint32, .Proto.GameCommand> commands = 2;
  int commands_size() const;
  void clear_commands();
  static const int kCommandsFieldNumber = 2;
  const ::google::protobuf::Map< ::google::protobuf::uint32, ::Proto::GameCommand >&
      commands() const;
  ::google::protobuf::Map< ::google::protobuf::uint32, ::Proto::GameCommand >*
      mutable_commands();

  // @@protoc_insertion_point(class_scope:Proto.GameCommandGroup)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  typedef ::google::protobuf::internal::MapEntryLite<
      ::google::protobuf::uint32, ::Proto::GameCommand,
      ::google::protobuf::internal::WireFormatLite::TYPE_UINT32,
      ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
      0 >
      GameCommandGroup_CommandsEntry;
  ::google::protobuf::internal::MapField<
      ::google::protobuf::uint32, ::Proto::GameCommand,
      ::google::protobuf::internal::WireFormatLite::TYPE_UINT32,
      ::google::protobuf::internal::WireFormatLite::TYPE_MESSAGE,
      0 > commands_;
  ::google::protobuf::uint32 frame_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static GameCommandGroup* default_instance_;
};
// -------------------------------------------------------------------

class Ping : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Proto.Ping) */ {
 public:
  Ping();
  virtual ~Ping();

  Ping(const Ping& from);

  inline Ping& operator=(const Ping& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Ping& default_instance();

  void Swap(Ping* other);

  // implements Message ----------------------------------------------

  inline Ping* New() const { return New(NULL); }

  Ping* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Ping& from);
  void MergeFrom(const Ping& from);
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
  void InternalSwap(Ping* other);
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

  // optional uint32 time = 1;
  void clear_time();
  static const int kTimeFieldNumber = 1;
  ::google::protobuf::uint32 time() const;
  void set_time(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:Proto.Ping)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::uint32 time_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Battle_2eproto();
  friend void protobuf_AssignDesc_Battle_2eproto();
  friend void protobuf_ShutdownFile_Battle_2eproto();

  void InitAsDefaultInstance();
  static Ping* default_instance_;
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
  // @@protoc_insertion_point(field_get:Proto.PlayerInfo.id)
  return id_;
}
inline void PlayerInfo::set_id(::google::protobuf::uint32 value) {
  
  id_ = value;
  // @@protoc_insertion_point(field_set:Proto.PlayerInfo.id)
}

// optional string actor_asset = 2;
inline void PlayerInfo::clear_actor_asset() {
  actor_asset_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PlayerInfo::actor_asset() const {
  // @@protoc_insertion_point(field_get:Proto.PlayerInfo.actor_asset)
  return actor_asset_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PlayerInfo::set_actor_asset(const ::std::string& value) {
  
  actor_asset_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Proto.PlayerInfo.actor_asset)
}
inline void PlayerInfo::set_actor_asset(const char* value) {
  
  actor_asset_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Proto.PlayerInfo.actor_asset)
}
inline void PlayerInfo::set_actor_asset(const char* value, size_t size) {
  
  actor_asset_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Proto.PlayerInfo.actor_asset)
}
inline ::std::string* PlayerInfo::mutable_actor_asset() {
  
  // @@protoc_insertion_point(field_mutable:Proto.PlayerInfo.actor_asset)
  return actor_asset_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PlayerInfo::release_actor_asset() {
  // @@protoc_insertion_point(field_release:Proto.PlayerInfo.actor_asset)
  
  return actor_asset_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PlayerInfo::set_allocated_actor_asset(::std::string* actor_asset) {
  if (actor_asset != NULL) {
    
  } else {
    
  }
  actor_asset_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), actor_asset);
  // @@protoc_insertion_point(field_set_allocated:Proto.PlayerInfo.actor_asset)
}

// -------------------------------------------------------------------

// GamePlayerInfos

// repeated .Proto.PlayerInfo player_infos = 1;
inline int GamePlayerInfos::player_infos_size() const {
  return player_infos_.size();
}
inline void GamePlayerInfos::clear_player_infos() {
  player_infos_.Clear();
}
inline const ::Proto::PlayerInfo& GamePlayerInfos::player_infos(int index) const {
  // @@protoc_insertion_point(field_get:Proto.GamePlayerInfos.player_infos)
  return player_infos_.Get(index);
}
inline ::Proto::PlayerInfo* GamePlayerInfos::mutable_player_infos(int index) {
  // @@protoc_insertion_point(field_mutable:Proto.GamePlayerInfos.player_infos)
  return player_infos_.Mutable(index);
}
inline ::Proto::PlayerInfo* GamePlayerInfos::add_player_infos() {
  // @@protoc_insertion_point(field_add:Proto.GamePlayerInfos.player_infos)
  return player_infos_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::Proto::PlayerInfo >*
GamePlayerInfos::mutable_player_infos() {
  // @@protoc_insertion_point(field_mutable_list:Proto.GamePlayerInfos.player_infos)
  return &player_infos_;
}
inline const ::google::protobuf::RepeatedPtrField< ::Proto::PlayerInfo >&
GamePlayerInfos::player_infos() const {
  // @@protoc_insertion_point(field_list:Proto.GamePlayerInfos.player_infos)
  return player_infos_;
}

// -------------------------------------------------------------------

// StartBattleInfo

// optional uint32 my_id = 1;
inline void StartBattleInfo::clear_my_id() {
  my_id_ = 0u;
}
inline ::google::protobuf::uint32 StartBattleInfo::my_id() const {
  // @@protoc_insertion_point(field_get:Proto.StartBattleInfo.my_id)
  return my_id_;
}
inline void StartBattleInfo::set_my_id(::google::protobuf::uint32 value) {
  
  my_id_ = value;
  // @@protoc_insertion_point(field_set:Proto.StartBattleInfo.my_id)
}

// optional .Proto.GamePlayerInfos player_infos = 2;
inline bool StartBattleInfo::has_player_infos() const {
  return !_is_default_instance_ && player_infos_ != NULL;
}
inline void StartBattleInfo::clear_player_infos() {
  if (GetArenaNoVirtual() == NULL && player_infos_ != NULL) delete player_infos_;
  player_infos_ = NULL;
}
inline const ::Proto::GamePlayerInfos& StartBattleInfo::player_infos() const {
  // @@protoc_insertion_point(field_get:Proto.StartBattleInfo.player_infos)
  return player_infos_ != NULL ? *player_infos_ : *default_instance_->player_infos_;
}
inline ::Proto::GamePlayerInfos* StartBattleInfo::mutable_player_infos() {
  
  if (player_infos_ == NULL) {
    player_infos_ = new ::Proto::GamePlayerInfos;
  }
  // @@protoc_insertion_point(field_mutable:Proto.StartBattleInfo.player_infos)
  return player_infos_;
}
inline ::Proto::GamePlayerInfos* StartBattleInfo::release_player_infos() {
  // @@protoc_insertion_point(field_release:Proto.StartBattleInfo.player_infos)
  
  ::Proto::GamePlayerInfos* temp = player_infos_;
  player_infos_ = NULL;
  return temp;
}
inline void StartBattleInfo::set_allocated_player_infos(::Proto::GamePlayerInfos* player_infos) {
  delete player_infos_;
  player_infos_ = player_infos;
  if (player_infos) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:Proto.StartBattleInfo.player_infos)
}

// optional uint32 start_time = 3;
inline void StartBattleInfo::clear_start_time() {
  start_time_ = 0u;
}
inline ::google::protobuf::uint32 StartBattleInfo::start_time() const {
  // @@protoc_insertion_point(field_get:Proto.StartBattleInfo.start_time)
  return start_time_;
}
inline void StartBattleInfo::set_start_time(::google::protobuf::uint32 value) {
  
  start_time_ = value;
  // @@protoc_insertion_point(field_set:Proto.StartBattleInfo.start_time)
}

// -------------------------------------------------------------------

// CS2CPlusPlusCommand

// optional float x_axis = 1;
inline void CS2CPlusPlusCommand::clear_x_axis() {
  x_axis_ = 0;
}
inline float CS2CPlusPlusCommand::x_axis() const {
  // @@protoc_insertion_point(field_get:Proto.CS2CPlusPlusCommand.x_axis)
  return x_axis_;
}
inline void CS2CPlusPlusCommand::set_x_axis(float value) {
  
  x_axis_ = value;
  // @@protoc_insertion_point(field_set:Proto.CS2CPlusPlusCommand.x_axis)
}

// optional float y_axis = 2;
inline void CS2CPlusPlusCommand::clear_y_axis() {
  y_axis_ = 0;
}
inline float CS2CPlusPlusCommand::y_axis() const {
  // @@protoc_insertion_point(field_get:Proto.CS2CPlusPlusCommand.y_axis)
  return y_axis_;
}
inline void CS2CPlusPlusCommand::set_y_axis(float value) {
  
  y_axis_ = value;
  // @@protoc_insertion_point(field_set:Proto.CS2CPlusPlusCommand.y_axis)
}

// optional bool jump = 3;
inline void CS2CPlusPlusCommand::clear_jump() {
  jump_ = false;
}
inline bool CS2CPlusPlusCommand::jump() const {
  // @@protoc_insertion_point(field_get:Proto.CS2CPlusPlusCommand.jump)
  return jump_;
}
inline void CS2CPlusPlusCommand::set_jump(bool value) {
  
  jump_ = value;
  // @@protoc_insertion_point(field_set:Proto.CS2CPlusPlusCommand.jump)
}

// optional int32 skill = 4;
inline void CS2CPlusPlusCommand::clear_skill() {
  skill_ = 0;
}
inline ::google::protobuf::int32 CS2CPlusPlusCommand::skill() const {
  // @@protoc_insertion_point(field_get:Proto.CS2CPlusPlusCommand.skill)
  return skill_;
}
inline void CS2CPlusPlusCommand::set_skill(::google::protobuf::int32 value) {
  
  skill_ = value;
  // @@protoc_insertion_point(field_set:Proto.CS2CPlusPlusCommand.skill)
}

// -------------------------------------------------------------------

// GameCommand

// optional uint64 x_axis = 1;
inline void GameCommand::clear_x_axis() {
  x_axis_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 GameCommand::x_axis() const {
  // @@protoc_insertion_point(field_get:Proto.GameCommand.x_axis)
  return x_axis_;
}
inline void GameCommand::set_x_axis(::google::protobuf::uint64 value) {
  
  x_axis_ = value;
  // @@protoc_insertion_point(field_set:Proto.GameCommand.x_axis)
}

// optional uint64 y_axis = 2;
inline void GameCommand::clear_y_axis() {
  y_axis_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 GameCommand::y_axis() const {
  // @@protoc_insertion_point(field_get:Proto.GameCommand.y_axis)
  return y_axis_;
}
inline void GameCommand::set_y_axis(::google::protobuf::uint64 value) {
  
  y_axis_ = value;
  // @@protoc_insertion_point(field_set:Proto.GameCommand.y_axis)
}

// optional bool jump = 3;
inline void GameCommand::clear_jump() {
  jump_ = false;
}
inline bool GameCommand::jump() const {
  // @@protoc_insertion_point(field_get:Proto.GameCommand.jump)
  return jump_;
}
inline void GameCommand::set_jump(bool value) {
  
  jump_ = value;
  // @@protoc_insertion_point(field_set:Proto.GameCommand.jump)
}

// optional int32 skill = 4;
inline void GameCommand::clear_skill() {
  skill_ = 0;
}
inline ::google::protobuf::int32 GameCommand::skill() const {
  // @@protoc_insertion_point(field_get:Proto.GameCommand.skill)
  return skill_;
}
inline void GameCommand::set_skill(::google::protobuf::int32 value) {
  
  skill_ = value;
  // @@protoc_insertion_point(field_set:Proto.GameCommand.skill)
}

// optional uint32 frame = 5;
inline void GameCommand::clear_frame() {
  frame_ = 0u;
}
inline ::google::protobuf::uint32 GameCommand::frame() const {
  // @@protoc_insertion_point(field_get:Proto.GameCommand.frame)
  return frame_;
}
inline void GameCommand::set_frame(::google::protobuf::uint32 value) {
  
  frame_ = value;
  // @@protoc_insertion_point(field_set:Proto.GameCommand.frame)
}

// -------------------------------------------------------------------

// GameCommandGroup

// optional uint32 frame = 1;
inline void GameCommandGroup::clear_frame() {
  frame_ = 0u;
}
inline ::google::protobuf::uint32 GameCommandGroup::frame() const {
  // @@protoc_insertion_point(field_get:Proto.GameCommandGroup.frame)
  return frame_;
}
inline void GameCommandGroup::set_frame(::google::protobuf::uint32 value) {
  
  frame_ = value;
  // @@protoc_insertion_point(field_set:Proto.GameCommandGroup.frame)
}

// map<uint32, .Proto.GameCommand> commands = 2;
inline int GameCommandGroup::commands_size() const {
  return commands_.size();
}
inline void GameCommandGroup::clear_commands() {
  commands_.Clear();
}
inline const ::google::protobuf::Map< ::google::protobuf::uint32, ::Proto::GameCommand >&
GameCommandGroup::commands() const {
  // @@protoc_insertion_point(field_map:Proto.GameCommandGroup.commands)
  return commands_.GetMap();
}
inline ::google::protobuf::Map< ::google::protobuf::uint32, ::Proto::GameCommand >*
GameCommandGroup::mutable_commands() {
  // @@protoc_insertion_point(field_mutable_map:Proto.GameCommandGroup.commands)
  return commands_.MutableMap();
}

// -------------------------------------------------------------------

// Ping

// optional uint32 time = 1;
inline void Ping::clear_time() {
  time_ = 0u;
}
inline ::google::protobuf::uint32 Ping::time() const {
  // @@protoc_insertion_point(field_get:Proto.Ping.time)
  return time_;
}
inline void Ping::set_time(::google::protobuf::uint32 value) {
  
  time_ = value;
  // @@protoc_insertion_point(field_set:Proto.Ping.time)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Proto

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Battle_2eproto__INCLUDED
