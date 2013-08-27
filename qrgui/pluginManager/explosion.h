#pragma once

#include <qrkernel/ids.h>

namespace qReal {

/// This class describes such entity as explosion: explosion relation between
/// @see source() and @see target() means that @see source() can or must
/// be binded with some instance of @see target() type. Double click on such
/// element unfolds a diagram with target element as root
class Explosion
{
public:
	Explosion(Id const &source, Id const &target
			, bool isReusable, bool requiresImmediateLinkage);

	/// The type of the source element
	Id source() const;

	/// The type of the target element
	Id target() const;

	/// True means that each instance of explosion binding will appear in
	/// special palette for future reusability
	bool isReusable() const;

	/// True means that elements of type @see source() have no sense without
	/// explosion binding so such binding must be created immediately after
	/// element creation
	bool requiresImmediateLinkage() const;

private:
	Id mSource;
	Id mTarget;
	bool mReusable;
	bool mRequiresImmediateLinkage;
};

}
