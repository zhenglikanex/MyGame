// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Battle.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Proto {

  /// <summary>Holder for reflection information generated from Battle.proto</summary>
  public static partial class BattleReflection {

    #region Descriptor
    /// <summary>File descriptor for Battle.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static BattleReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "CgxCYXR0bGUucHJvdG8SBVByb3RvIi0KClBsYXllckluZm8SCgoCaWQYASAB",
            "KA0SEwoLYWN0b3JfYXNzZXQYAiABKAkiOgoPR2FtZVBsYXllckluZm9zEicK",
            "DHBsYXllcl9pbmZvcxgBIAMoCzIRLlByb3RvLlBsYXllckluZm8iYgoPU3Rh",
            "cnRCYXR0bGVJbmZvEg0KBW15X2lkGAEgASgNEiwKDHBsYXllcl9pbmZvcxgC",
            "IAEoCzIWLlByb3RvLkdhbWVQbGF5ZXJJbmZvcxISCgpzdGFydF90aW1lGAMg",
            "ASgNIlIKE0NTMkNQbHVzUGx1c0NvbW1hbmQSDgoGeF9heGlzGAEgASgCEg4K",
            "BnlfYXhpcxgCIAEoAhIMCgRqdW1wGAMgASgIEg0KBXNraWxsGAQgASgFIlkK",
            "C0dhbWVDb21tYW5kEg4KBnhfYXhpcxgBIAEoBBIOCgZ5X2F4aXMYAiABKAQS",
            "DAoEanVtcBgDIAEoCBINCgVza2lsbBgEIAEoBRINCgVmcmFtZRgFIAEoDSKf",
            "AQoQR2FtZUNvbW1hbmRHcm91cBINCgVmcmFtZRgBIAEoDRI3Cghjb21tYW5k",
            "cxgCIAMoCzIlLlByb3RvLkdhbWVDb21tYW5kR3JvdXAuQ29tbWFuZHNFbnRy",
            "eRpDCg1Db21tYW5kc0VudHJ5EgsKA2tleRgBIAEoDRIhCgV2YWx1ZRgCIAEo",
            "CzISLlByb3RvLkdhbWVDb21tYW5kOgI4ASIUCgRQaW5nEgwKBHRpbWUYASAB",
            "KA1iBnByb3RvMw=="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Proto.PlayerInfo), global::Proto.PlayerInfo.Parser, new[]{ "Id", "ActorAsset" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Proto.GamePlayerInfos), global::Proto.GamePlayerInfos.Parser, new[]{ "PlayerInfos" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Proto.StartBattleInfo), global::Proto.StartBattleInfo.Parser, new[]{ "MyId", "PlayerInfos", "StartTime" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Proto.CS2CPlusPlusCommand), global::Proto.CS2CPlusPlusCommand.Parser, new[]{ "XAxis", "YAxis", "Jump", "Skill" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Proto.GameCommand), global::Proto.GameCommand.Parser, new[]{ "XAxis", "YAxis", "Jump", "Skill", "Frame" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Proto.GameCommandGroup), global::Proto.GameCommandGroup.Parser, new[]{ "Frame", "Commands" }, null, null, new pbr::GeneratedClrTypeInfo[] { null, }),
            new pbr::GeneratedClrTypeInfo(typeof(global::Proto.Ping), global::Proto.Ping.Parser, new[]{ "Time" }, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  public sealed partial class PlayerInfo : pb::IMessage<PlayerInfo> {
    private static readonly pb::MessageParser<PlayerInfo> _parser = new pb::MessageParser<PlayerInfo>(() => new PlayerInfo());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<PlayerInfo> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Proto.BattleReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public PlayerInfo() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public PlayerInfo(PlayerInfo other) : this() {
      id_ = other.id_;
      actorAsset_ = other.actorAsset_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public PlayerInfo Clone() {
      return new PlayerInfo(this);
    }

    /// <summary>Field number for the "id" field.</summary>
    public const int IdFieldNumber = 1;
    private uint id_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint Id {
      get { return id_; }
      set {
        id_ = value;
      }
    }

    /// <summary>Field number for the "actor_asset" field.</summary>
    public const int ActorAssetFieldNumber = 2;
    private string actorAsset_ = "";
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string ActorAsset {
      get { return actorAsset_; }
      set {
        actorAsset_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as PlayerInfo);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(PlayerInfo other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Id != other.Id) return false;
      if (ActorAsset != other.ActorAsset) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Id != 0) hash ^= Id.GetHashCode();
      if (ActorAsset.Length != 0) hash ^= ActorAsset.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Id != 0) {
        output.WriteRawTag(8);
        output.WriteUInt32(Id);
      }
      if (ActorAsset.Length != 0) {
        output.WriteRawTag(18);
        output.WriteString(ActorAsset);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Id != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(Id);
      }
      if (ActorAsset.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(ActorAsset);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(PlayerInfo other) {
      if (other == null) {
        return;
      }
      if (other.Id != 0) {
        Id = other.Id;
      }
      if (other.ActorAsset.Length != 0) {
        ActorAsset = other.ActorAsset;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            Id = input.ReadUInt32();
            break;
          }
          case 18: {
            ActorAsset = input.ReadString();
            break;
          }
        }
      }
    }

  }

  public sealed partial class GamePlayerInfos : pb::IMessage<GamePlayerInfos> {
    private static readonly pb::MessageParser<GamePlayerInfos> _parser = new pb::MessageParser<GamePlayerInfos>(() => new GamePlayerInfos());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<GamePlayerInfos> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Proto.BattleReflection.Descriptor.MessageTypes[1]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GamePlayerInfos() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GamePlayerInfos(GamePlayerInfos other) : this() {
      playerInfos_ = other.playerInfos_.Clone();
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GamePlayerInfos Clone() {
      return new GamePlayerInfos(this);
    }

    /// <summary>Field number for the "player_infos" field.</summary>
    public const int PlayerInfosFieldNumber = 1;
    private static readonly pb::FieldCodec<global::Proto.PlayerInfo> _repeated_playerInfos_codec
        = pb::FieldCodec.ForMessage(10, global::Proto.PlayerInfo.Parser);
    private readonly pbc::RepeatedField<global::Proto.PlayerInfo> playerInfos_ = new pbc::RepeatedField<global::Proto.PlayerInfo>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<global::Proto.PlayerInfo> PlayerInfos {
      get { return playerInfos_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as GamePlayerInfos);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(GamePlayerInfos other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if(!playerInfos_.Equals(other.playerInfos_)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      hash ^= playerInfos_.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      playerInfos_.WriteTo(output, _repeated_playerInfos_codec);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      size += playerInfos_.CalculateSize(_repeated_playerInfos_codec);
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(GamePlayerInfos other) {
      if (other == null) {
        return;
      }
      playerInfos_.Add(other.playerInfos_);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 10: {
            playerInfos_.AddEntriesFrom(input, _repeated_playerInfos_codec);
            break;
          }
        }
      }
    }

  }

  public sealed partial class StartBattleInfo : pb::IMessage<StartBattleInfo> {
    private static readonly pb::MessageParser<StartBattleInfo> _parser = new pb::MessageParser<StartBattleInfo>(() => new StartBattleInfo());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<StartBattleInfo> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Proto.BattleReflection.Descriptor.MessageTypes[2]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public StartBattleInfo() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public StartBattleInfo(StartBattleInfo other) : this() {
      myId_ = other.myId_;
      PlayerInfos = other.playerInfos_ != null ? other.PlayerInfos.Clone() : null;
      startTime_ = other.startTime_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public StartBattleInfo Clone() {
      return new StartBattleInfo(this);
    }

    /// <summary>Field number for the "my_id" field.</summary>
    public const int MyIdFieldNumber = 1;
    private uint myId_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint MyId {
      get { return myId_; }
      set {
        myId_ = value;
      }
    }

    /// <summary>Field number for the "player_infos" field.</summary>
    public const int PlayerInfosFieldNumber = 2;
    private global::Proto.GamePlayerInfos playerInfos_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Proto.GamePlayerInfos PlayerInfos {
      get { return playerInfos_; }
      set {
        playerInfos_ = value;
      }
    }

    /// <summary>Field number for the "start_time" field.</summary>
    public const int StartTimeFieldNumber = 3;
    private uint startTime_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint StartTime {
      get { return startTime_; }
      set {
        startTime_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as StartBattleInfo);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(StartBattleInfo other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (MyId != other.MyId) return false;
      if (!object.Equals(PlayerInfos, other.PlayerInfos)) return false;
      if (StartTime != other.StartTime) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (MyId != 0) hash ^= MyId.GetHashCode();
      if (playerInfos_ != null) hash ^= PlayerInfos.GetHashCode();
      if (StartTime != 0) hash ^= StartTime.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (MyId != 0) {
        output.WriteRawTag(8);
        output.WriteUInt32(MyId);
      }
      if (playerInfos_ != null) {
        output.WriteRawTag(18);
        output.WriteMessage(PlayerInfos);
      }
      if (StartTime != 0) {
        output.WriteRawTag(24);
        output.WriteUInt32(StartTime);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (MyId != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(MyId);
      }
      if (playerInfos_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(PlayerInfos);
      }
      if (StartTime != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(StartTime);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(StartBattleInfo other) {
      if (other == null) {
        return;
      }
      if (other.MyId != 0) {
        MyId = other.MyId;
      }
      if (other.playerInfos_ != null) {
        if (playerInfos_ == null) {
          playerInfos_ = new global::Proto.GamePlayerInfos();
        }
        PlayerInfos.MergeFrom(other.PlayerInfos);
      }
      if (other.StartTime != 0) {
        StartTime = other.StartTime;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            MyId = input.ReadUInt32();
            break;
          }
          case 18: {
            if (playerInfos_ == null) {
              playerInfos_ = new global::Proto.GamePlayerInfos();
            }
            input.ReadMessage(playerInfos_);
            break;
          }
          case 24: {
            StartTime = input.ReadUInt32();
            break;
          }
        }
      }
    }

  }

  /// <summary>
  /// 偷个懒用这个来做csharp到c++的命令传输
  /// </summary>
  public sealed partial class CS2CPlusPlusCommand : pb::IMessage<CS2CPlusPlusCommand> {
    private static readonly pb::MessageParser<CS2CPlusPlusCommand> _parser = new pb::MessageParser<CS2CPlusPlusCommand>(() => new CS2CPlusPlusCommand());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<CS2CPlusPlusCommand> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Proto.BattleReflection.Descriptor.MessageTypes[3]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public CS2CPlusPlusCommand() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public CS2CPlusPlusCommand(CS2CPlusPlusCommand other) : this() {
      xAxis_ = other.xAxis_;
      yAxis_ = other.yAxis_;
      jump_ = other.jump_;
      skill_ = other.skill_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public CS2CPlusPlusCommand Clone() {
      return new CS2CPlusPlusCommand(this);
    }

    /// <summary>Field number for the "x_axis" field.</summary>
    public const int XAxisFieldNumber = 1;
    private float xAxis_;
    /// <summary>
    ///  摇杆输入
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public float XAxis {
      get { return xAxis_; }
      set {
        xAxis_ = value;
      }
    }

    /// <summary>Field number for the "y_axis" field.</summary>
    public const int YAxisFieldNumber = 2;
    private float yAxis_;
    /// <summary>
    ///  摇杆输入
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public float YAxis {
      get { return yAxis_; }
      set {
        yAxis_ = value;
      }
    }

    /// <summary>Field number for the "jump" field.</summary>
    public const int JumpFieldNumber = 3;
    private bool jump_;
    /// <summary>
    ///  其他按键的一些输入,如跳跃,攻击啥的一些键位是否按下
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Jump {
      get { return jump_; }
      set {
        jump_ = value;
      }
    }

    /// <summary>Field number for the "skill" field.</summary>
    public const int SkillFieldNumber = 4;
    private int skill_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int Skill {
      get { return skill_; }
      set {
        skill_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as CS2CPlusPlusCommand);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(CS2CPlusPlusCommand other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (XAxis != other.XAxis) return false;
      if (YAxis != other.YAxis) return false;
      if (Jump != other.Jump) return false;
      if (Skill != other.Skill) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (XAxis != 0F) hash ^= XAxis.GetHashCode();
      if (YAxis != 0F) hash ^= YAxis.GetHashCode();
      if (Jump != false) hash ^= Jump.GetHashCode();
      if (Skill != 0) hash ^= Skill.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (XAxis != 0F) {
        output.WriteRawTag(13);
        output.WriteFloat(XAxis);
      }
      if (YAxis != 0F) {
        output.WriteRawTag(21);
        output.WriteFloat(YAxis);
      }
      if (Jump != false) {
        output.WriteRawTag(24);
        output.WriteBool(Jump);
      }
      if (Skill != 0) {
        output.WriteRawTag(32);
        output.WriteInt32(Skill);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (XAxis != 0F) {
        size += 1 + 4;
      }
      if (YAxis != 0F) {
        size += 1 + 4;
      }
      if (Jump != false) {
        size += 1 + 1;
      }
      if (Skill != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Skill);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(CS2CPlusPlusCommand other) {
      if (other == null) {
        return;
      }
      if (other.XAxis != 0F) {
        XAxis = other.XAxis;
      }
      if (other.YAxis != 0F) {
        YAxis = other.YAxis;
      }
      if (other.Jump != false) {
        Jump = other.Jump;
      }
      if (other.Skill != 0) {
        Skill = other.Skill;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 13: {
            XAxis = input.ReadFloat();
            break;
          }
          case 21: {
            YAxis = input.ReadFloat();
            break;
          }
          case 24: {
            Jump = input.ReadBool();
            break;
          }
          case 32: {
            Skill = input.ReadInt32();
            break;
          }
        }
      }
    }

  }

  public sealed partial class GameCommand : pb::IMessage<GameCommand> {
    private static readonly pb::MessageParser<GameCommand> _parser = new pb::MessageParser<GameCommand>(() => new GameCommand());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<GameCommand> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Proto.BattleReflection.Descriptor.MessageTypes[4]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GameCommand() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GameCommand(GameCommand other) : this() {
      xAxis_ = other.xAxis_;
      yAxis_ = other.yAxis_;
      jump_ = other.jump_;
      skill_ = other.skill_;
      frame_ = other.frame_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GameCommand Clone() {
      return new GameCommand(this);
    }

    /// <summary>Field number for the "x_axis" field.</summary>
    public const int XAxisFieldNumber = 1;
    private ulong xAxis_;
    /// <summary>
    ///  摇杆输入
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ulong XAxis {
      get { return xAxis_; }
      set {
        xAxis_ = value;
      }
    }

    /// <summary>Field number for the "y_axis" field.</summary>
    public const int YAxisFieldNumber = 2;
    private ulong yAxis_;
    /// <summary>
    ///  摇杆输入
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ulong YAxis {
      get { return yAxis_; }
      set {
        yAxis_ = value;
      }
    }

    /// <summary>Field number for the "jump" field.</summary>
    public const int JumpFieldNumber = 3;
    private bool jump_;
    /// <summary>
    ///  其他按键的一些输入,如跳跃,攻击啥的一些键位是否按下
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Jump {
      get { return jump_; }
      set {
        jump_ = value;
      }
    }

    /// <summary>Field number for the "skill" field.</summary>
    public const int SkillFieldNumber = 4;
    private int skill_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int Skill {
      get { return skill_; }
      set {
        skill_ = value;
      }
    }

    /// <summary>Field number for the "frame" field.</summary>
    public const int FrameFieldNumber = 5;
    private uint frame_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint Frame {
      get { return frame_; }
      set {
        frame_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as GameCommand);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(GameCommand other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (XAxis != other.XAxis) return false;
      if (YAxis != other.YAxis) return false;
      if (Jump != other.Jump) return false;
      if (Skill != other.Skill) return false;
      if (Frame != other.Frame) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (XAxis != 0UL) hash ^= XAxis.GetHashCode();
      if (YAxis != 0UL) hash ^= YAxis.GetHashCode();
      if (Jump != false) hash ^= Jump.GetHashCode();
      if (Skill != 0) hash ^= Skill.GetHashCode();
      if (Frame != 0) hash ^= Frame.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (XAxis != 0UL) {
        output.WriteRawTag(8);
        output.WriteUInt64(XAxis);
      }
      if (YAxis != 0UL) {
        output.WriteRawTag(16);
        output.WriteUInt64(YAxis);
      }
      if (Jump != false) {
        output.WriteRawTag(24);
        output.WriteBool(Jump);
      }
      if (Skill != 0) {
        output.WriteRawTag(32);
        output.WriteInt32(Skill);
      }
      if (Frame != 0) {
        output.WriteRawTag(40);
        output.WriteUInt32(Frame);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (XAxis != 0UL) {
        size += 1 + pb::CodedOutputStream.ComputeUInt64Size(XAxis);
      }
      if (YAxis != 0UL) {
        size += 1 + pb::CodedOutputStream.ComputeUInt64Size(YAxis);
      }
      if (Jump != false) {
        size += 1 + 1;
      }
      if (Skill != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Skill);
      }
      if (Frame != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(Frame);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(GameCommand other) {
      if (other == null) {
        return;
      }
      if (other.XAxis != 0UL) {
        XAxis = other.XAxis;
      }
      if (other.YAxis != 0UL) {
        YAxis = other.YAxis;
      }
      if (other.Jump != false) {
        Jump = other.Jump;
      }
      if (other.Skill != 0) {
        Skill = other.Skill;
      }
      if (other.Frame != 0) {
        Frame = other.Frame;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            XAxis = input.ReadUInt64();
            break;
          }
          case 16: {
            YAxis = input.ReadUInt64();
            break;
          }
          case 24: {
            Jump = input.ReadBool();
            break;
          }
          case 32: {
            Skill = input.ReadInt32();
            break;
          }
          case 40: {
            Frame = input.ReadUInt32();
            break;
          }
        }
      }
    }

  }

  public sealed partial class GameCommandGroup : pb::IMessage<GameCommandGroup> {
    private static readonly pb::MessageParser<GameCommandGroup> _parser = new pb::MessageParser<GameCommandGroup>(() => new GameCommandGroup());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<GameCommandGroup> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Proto.BattleReflection.Descriptor.MessageTypes[5]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GameCommandGroup() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GameCommandGroup(GameCommandGroup other) : this() {
      frame_ = other.frame_;
      commands_ = other.commands_.Clone();
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public GameCommandGroup Clone() {
      return new GameCommandGroup(this);
    }

    /// <summary>Field number for the "frame" field.</summary>
    public const int FrameFieldNumber = 1;
    private uint frame_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint Frame {
      get { return frame_; }
      set {
        frame_ = value;
      }
    }

    /// <summary>Field number for the "commands" field.</summary>
    public const int CommandsFieldNumber = 2;
    private static readonly pbc::MapField<uint, global::Proto.GameCommand>.Codec _map_commands_codec
        = new pbc::MapField<uint, global::Proto.GameCommand>.Codec(pb::FieldCodec.ForUInt32(8), pb::FieldCodec.ForMessage(18, global::Proto.GameCommand.Parser), 18);
    private readonly pbc::MapField<uint, global::Proto.GameCommand> commands_ = new pbc::MapField<uint, global::Proto.GameCommand>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::MapField<uint, global::Proto.GameCommand> Commands {
      get { return commands_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as GameCommandGroup);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(GameCommandGroup other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Frame != other.Frame) return false;
      if (!Commands.Equals(other.Commands)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Frame != 0) hash ^= Frame.GetHashCode();
      hash ^= Commands.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Frame != 0) {
        output.WriteRawTag(8);
        output.WriteUInt32(Frame);
      }
      commands_.WriteTo(output, _map_commands_codec);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Frame != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(Frame);
      }
      size += commands_.CalculateSize(_map_commands_codec);
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(GameCommandGroup other) {
      if (other == null) {
        return;
      }
      if (other.Frame != 0) {
        Frame = other.Frame;
      }
      commands_.Add(other.commands_);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            Frame = input.ReadUInt32();
            break;
          }
          case 18: {
            commands_.AddEntriesFrom(input, _map_commands_codec);
            break;
          }
        }
      }
    }

  }

  public sealed partial class Ping : pb::IMessage<Ping> {
    private static readonly pb::MessageParser<Ping> _parser = new pb::MessageParser<Ping>(() => new Ping());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Ping> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Proto.BattleReflection.Descriptor.MessageTypes[6]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Ping() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Ping(Ping other) : this() {
      time_ = other.time_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Ping Clone() {
      return new Ping(this);
    }

    /// <summary>Field number for the "time" field.</summary>
    public const int TimeFieldNumber = 1;
    private uint time_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public uint Time {
      get { return time_; }
      set {
        time_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Ping);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Ping other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Time != other.Time) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Time != 0) hash ^= Time.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Time != 0) {
        output.WriteRawTag(8);
        output.WriteUInt32(Time);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Time != 0) {
        size += 1 + pb::CodedOutputStream.ComputeUInt32Size(Time);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Ping other) {
      if (other == null) {
        return;
      }
      if (other.Time != 0) {
        Time = other.Time;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            Time = input.ReadUInt32();
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
