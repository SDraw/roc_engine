#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <array>
#include <cstdint>
#include <ctime>

#include "lua.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "pugixml.hpp"

#include "IAnimation.h"
#include "ICamera.h"
#include "ICollidable.h"
#include "ICollider.h"
#include "ICore.h"
#include "ICustomArgument.h"
#include "ICustomArguments.h"
#include "IDrawable.h"
#include "IElement.h"
#include "IElementManager.h"
#include "IFont.h"
#include "ILight.h"
#include "ILogManager.h"
#include "IMesh.h"
#include "IModel.h"
#include "IModule.h"
#include "IPhysicsManager.h"
#include "IRenderManager.h"
#include "IRenderTarget.h"
#include "IScene.h"
#include "ISfmlManager.h"
#include "IShader.h"
#include "ISound.h"
#include "ISoundManager.h"
#include "ITexture.h"
#include "ITransformable.h"
