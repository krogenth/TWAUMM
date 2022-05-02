#pragma once

#include <vector>
#include <deque>

class Tribe;
class Player;
class Village;

template<class T>
T* searchVectorByID(std::vector<T>& objects, const size_t& id) {
	auto iter = std::find_if(objects.begin(), objects.end(), [=](T& object) {
			return (object.getID() == id);
		});

	if (iter != objects.end()) {
		return &(*iter);
	}
	else {
		return nullptr;
	}
}

template<class T>
const T* searchVectorByID(const std::vector<T>& objects, const size_t& id) {
	auto iter = std::find_if(objects.begin(), objects.end(), [=](const T& object) {
		return (object.getID() == id);
		});

	if (iter != objects.end()) {
		return &(*iter);
	}
	else {
		return nullptr;
	}
}

template<class T>
T* searchDequeByID(std::deque<T>& objects, const size_t& id) {
	auto iter = std::find_if(objects.begin(), objects.end(), [=](T& object) {
		return (object.getID() == id);
		});

	if (iter != objects.end()) {
		return &(*iter);
	}
	else {
		return nullptr;
	}
}

template<class T>
const T* searchDequeByID(const std::deque<T>& objects, const size_t& id) {
	auto iter = std::find_if(objects.begin(), objects.end(), [=](const T& object) {
		return (object.getID() == id);
		});

	if (iter != objects.end()) {
		return &(*iter);
	}
	else {
		return nullptr;
	}
}