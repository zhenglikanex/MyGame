#pragma once

#include <cassert>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/GameHelper.hpp"

#include "Framework/Game/Component/EntityByLocalMap.hpp"
#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/SyncManager.hpp"

#include "3rdparty/include/entt/entt.hpp"

// 包装一下entt::registry,方便管理unique和local
class Registry
{
public:
	Registry()
		: local_map_(registry_.set<EntityByLocalMap>())
		, unique_map_(registry_.set<EntityByUniqueIdMap>())
		, game_helper_(registry_.ctx<Locator>().Ref<GameHelper>())
	{
		registry_.on_construct<Local>().connect<&ConnectLocalIdToEntity>();
		registry_.on_destroy<Local>().connect<&DisconnectLocalIdToEntity>();

		registry_.on_construct<UniqueId>().connect<&ConnectUniqueIdToEntity>();
		registry_.on_destroy<UniqueId>().connect<&DisconnectUniqueIdToEntity>();
	}

	[[nodiscard]] bool valid(const entt::registry::entity_type entity) const {
		return registry_.valid(entity);
	}

	[[nodiscard]] entt::registry::entity_type create() {
		return registry_.create();
	}

	entt::registry::version_type destroy(const entt::registry::entity_type entity) {
		return registry_.destroy(entity);
	}

	template<typename It>
	void destroy(It first, It last) {
		registry_.destroy(first, last);
	}

	template<typename Component, typename... Args>
	decltype(auto) emplace(const entt::registry::entity_type entity, Args &&...args) {
		return registry.emplace(entity, std::forward(args)...);
	}

	template<typename Component, typename It>
	void insert(It first, It last, const Component &value = {}) {
		registry_.insert(first, last, value);
	}

	template<typename Component, typename EIt, typename CIt, typename = std::enable_if_t<std::is_same_v<std::decay_t<typename std::iterator_traits<CIt>::value_type>, Component>>>
	void insert(EIt first, EIt last, CIt from) {
		registry_.insert(first, last, from);
	}

	template<typename Component, typename... Args>
	decltype(auto) emplace_or_replace(const entt::registry::entity_type entity, Args &&...args) {
		registry_.emplace_or_replace(entity, std::forward(args)...);
	}

	template<typename Component, typename... Func>
	decltype(auto) patch(const entt::registry::entity_type entity, Func &&...func) {
		registry_.patch(entity, std::forward(func)...);
	}

	template<typename Component, typename... Args>
	decltype(auto) replace(const entt::registry::entity_type entity, Args &&...args) {
		registry_.replace(entity, std::forward(args)...);
	}

	template<typename Component, typename... Other>
	entt::registry::size_type remove(const entt::registry::entity_type entity) {
		return registry_.remove(entity);
	}

	template<typename Component, typename... Other, typename It>
	entt::registry::size_type remove(It first, It last) {
		return registry_.remove(first, last);
	}

	template<typename... Component>
	[[nodiscard]] bool all_of(const entt::registry::entity_type entity) const {
		return registry_.all_of<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] bool any_of(const entt::registry::entity_type entity) const {
		return registry_.any_of<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] decltype(auto) get([[maybe_unused]] const entt::registry::entity_type entity) const {
		return registry_.get<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] decltype(auto) get([[maybe_unused]] const entt::registry::entity_type entity) {
		return registry_.get<Component...>(entity);
	}

	template<typename Component, typename... Args>
	[[nodiscard]] decltype(auto) get_or_emplace(const entt::registry::entity_type entity, Args &&...args) {
		return get_or_emplace<Component>(entity, std::forward(args)...);
	}

	template<typename... Component>
	[[nodiscard]] auto try_get([[maybe_unused]] const entt::registry::entity_type entity) const {
		return registry_.try_get<Component...>(entity);
	}

	template<typename... Component>
	[[nodiscard]] auto try_get([[maybe_unused]] const entt::registry::entity_type entity) {
		return registry_.try_get<Component...>(entity);
	}

	template<typename... Component>
	void clear() {
		registry_.clear<Component...>();
	}

	template<typename Func>
	void each(Func func) const {
		registry_.each(std::forward(func));
	}

	template<typename Component>
	[[nodiscard]] auto on_construct() {
		return registry_.on_construct<Component>();
	}

	template<typename Component>
	[[nodiscard]] auto on_update() {
		return registry_.on_update<Component>();
	}

	template<typename Component>
	[[nodiscard]] auto on_destroy() {
		return registry_.on_destroy<Component>();
	}

	template<typename Component, typename... Other, typename... Exclude>
	[[nodiscard]] entt::basic_view<entt::entity, entt::get_t<std::add_const_t<Component>, std::add_const_t<Other>...>, entt::exclude_t<Exclude...>> view() const {
		return registry_.view<Component, Other..., Exclude...>();
	}

	template<typename Component, typename... Other, typename... Exclude>
	[[nodiscard]] entt::basic_view<entt::entity, entt::get_t<Component, Other...>, entt::exclude_t<Exclude...>> view() {
		return registry_.view<Component, Other..., Exclude...>();
	}

	template<typename... Owned, typename... Get, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<Owned...>, entt::get_t<Get...>, entt::exclude_t<Exclude...>> group() {
		return registry_.group<Owned..., Get..., Exclude...>();
	}

	template<typename... Owned, typename... Get, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<std::add_const_t<Owned>...>, entt::get_t<std::add_const_t<Get>...>, entt::exclude_t<Exclude...>> group_if_exists() const {
		return registry_.group<Owned..., Get..., Exclude...>();
	}

	template<typename... Owned, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<Owned...>, entt::get_t<>, entt::exclude_t<Exclude...>> group() {
		return registry_.group<Owned...,Exclude...>();
	}

	template<typename... Owned, typename... Exclude>
	[[nodiscard]] entt::basic_group<entt::entity, entt::owned_t<std::add_const_t<Owned>...>, entt::get_t<>, entt::exclude_t<Exclude...>> group_if_exists() const {
		return registry_.group_if_exists<Owned..., Exclude...>();
	}

	template<typename... Component>
	[[nodiscard]] bool sortable() const {
		return registry_.sortable<Component...>();
	}

	template<typename... Owned, typename... Get, typename... Exclude>
	[[nodiscard]] bool sortable() ENTT_NOEXCEPT {
		return registry_.sortable<Owned..., Get..., Exclude...>();
	}

	template<typename Component, typename Compare, typename Sort = entt::std_sort, typename... Args>
	void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
		registry_.sort<Component>(std::forward(compare), std::forward(algo), std::forward(args)...);
	}

	template<typename To, typename From>
	void sort() {
		registry_.sort<To, From>();
	}

	template<typename Type, typename... Args>
	Type &set(Args &&...args) {
		return registry_.set<Type>(std::forward(args)...);
	}

	template<typename Type>
	void unset() {
		return registry_.unset<Type>();
	}

	template<typename Type, typename... Args>
	[[nodiscard]] Type &ctx_or_set(Args &&...args) {
		return registry_.ctx<>
	}

	template<typename Type>
	[[nodiscard]] std::add_const_t<Type> *try_ctx() const {
		return registry_.try_ctx<Type>();
	}

	template<typename Type>
	[[nodiscard]] Type *try_ctx() {
		return registry_.try_ctx<Type>();
	}

	template<typename Type>
	[[nodiscard]] std::add_const_t<Type> &ctx() const {
		return registry_.ctx<Type>();
	}

	/*! @copydoc ctx */
	template<typename Type>
	[[nodiscard]] Type &ctx() {
		return registry_.ctx<Type>();
	}

	template<typename Func>
	void ctx(Func func) const {
		registry_.ctx(std::forward(func));
	}

public:
	entt::entity create_predict(uint32_t player) const
	{
		auto e = registry_.create();
		if (game_helper_.IsAuthority())
		{
			registry_.emplace<UniqueId>(e, e);
		}

		auto& sync_manager = registry_.ctx<SyncManager>();
		auto& sync = sync_manager.sync_instances[player];
		registry_.emplace<Local>(e, sync.owner, sync.identifier++);

		return e;
	}

	// 根据source预测创建可以在客户端之间同步的实体
	entt::entity create_predict(entt::entity source) const
	{
		if (auto player = registry_.try_get<Player>(source))
		{
			// 客户端控制的实体
			return create_predict(player->id);
		}
		else
		{
			return create_predict(ServerInstance);
		}
	}

	entt::entity create_predict() const
	{
		return create_predict(ServerInstance);
	}

	std::tuple<entt::entity,UniqueId> create_unique() const 
	{
		auto e = registry_.create();
		auto& unique_id = registry_.emplace<UniqueId>(e, e);
		return { e,unique_id };
	}

	// 根据source预测创建本地实体,不在客户端之间同步
	std::tuple<entt::entity,Local> create_local(entt::entity source) const
	{
		if (auto player = registry_.try_get<Player>(source))
		{
			// 客户端控制的实体
			auto e = registry_.create();
			auto& sync_manager = registry_.ctx<SyncManager>();
			auto& sync = sync_manager.sync_instances[player->id];
			auto& local = registry_.emplace<Local>(e, sync.owner, sync.identifier++);

			return { e,local };
		}
		
		return create_local();
	}

	// 创建服务端本地实体
	std::tuple<entt::entity, Local> create_local() const
	{
		//服务端控制的实体
		auto e = registry_.create();
		auto& sync_manager = registry_.ctx<SyncManager>();
		auto& sync = sync_manager.sync_instances[ServerInstance];
		registry_.emplace<Local>(e, sync.owner, sync.identifier++);
	}

	bool valid(UniqueId id) const
	{
		auto it = unique_map_.value.find(id);
		if (it == unique_map_.value.end())
		{
			return false;
		}

		return registry_.valid(it->second);
	}

	bool valid(Local id) const 
	{
		auto it = local_map_.value.find(id);
		if (it == local_map_.value.end())
		{
			return false;
		}

		return registry_.valid(it->second);
	}

	entt::entity get(UniqueId id) const
	{
		assert(valid(id));
		
		return unique_map_.value.find(id)->second;
	}

	entt::entity get(const Local& local) const
	{
		assert(valid(local));

		return local_map_.value.find(local)->second;
	}

	entt::entity try_get(UniqueId id) const
	{
		if (valid(id))
		{
			return get(id);
		}
		return entt::null;
	}

	entt::entity try_get(const Local& local) const
	{
		if (valid(local))
		{
			return get(local);
		}
		return entt::null;
	}

private:
	static void ConnectUniqueIdToEntity(Registry& registry, entt::entity e)
	{
		auto& unique_id = registry.get<UniqueId>(e);

		auto& map = registry.ctx<EntityByUniqueIdMap>();
		map.value.emplace(unique_id, e);
	}

	static void DisconnectUniqueIdToEntity(Registry& registry, entt::entity e)
	{
		auto& unique_id = registry.get<UniqueId>(e);

		auto& map = registry.ctx<EntityByUniqueIdMap>();
		map.value.erase(unique_id);
	}

	static void ConnectLocalIdToEntity(Registry& registry, entt::entity e)
	{
		auto& local_id = registry.get<Local>(e);

		auto& map = registry.ctx<EntityByLocalMap>();
		map.value.emplace(local_id, e);
	}

	static void DisconnectLocalIdToEntity(Registry& registry, entt::entity e)
	{
		auto& local_id = registry.get<Local>(e);

		auto& map = registry.ctx<EntityByLocalMap>();
		map.value.erase(local_id);
	}
private:
	entt::registry registry_;
	EntityByLocalMap& local_map_;
	EntityByUniqueIdMap& unique_map_;
	GameHelper& game_helper_;
};