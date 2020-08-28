#pragma once

#include <vector>
#include <algorithm>

#include <tdme/tdme.h>
#include <tdme/utils/fwd-tdme.h>

using std::vector;
using std::find;

namespace tdme {
namespace utils {

/** 
 * Object deleter
 * @author Andreas Drewke
 * @version $Id$
 */
template<typename T>
class ObjectDeleter final
{
private:
	vector<T*> objects;

public:

	/** 
	 * Adds vector to iterator
	 * @param vector vector
	 */
	T* add(T* object) {
		if (find(objects.begin(), objects.end(), object) != objects.end()) return object;
		objects.push_back(object);
		return object;
	}

	/**
	 * Public constructor
	 */
	ObjectDeleter() {
	}

	/**
	 * Public destructor
	 */
	~ObjectDeleter() {
		for (auto object: objects) delete object;
		objects.clear();
	}

};

};
};