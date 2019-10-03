//
// Created by matth on 30/03/2019.
//

#include "NovelBasicInteractionRect.h"
#include "GeoBounds.h"
namespace NovelRT {
NovelBasicInteractionRect::NovelBasicInteractionRect(NovelLayeringService* layeringService,
                                                     const GeoVector<float>& size,
                                                     const NovelCommonArgs& args,
                                                     const std::function<void(NovelInteractionObject*)> notifyHasBeenDrawnObject)
    : NovelInteractionObject(layeringService, size, args, notifyHasBeenDrawnObject) {

}
bool NovelBasicInteractionRect::validateInteractionPerimeter(const NovelRT::GeoVector<float>& mousePosition) const {
  GeoVector<float> position = getPosition(); // * _screenScale;
  GeoVector<float> size = getSize(); // * _screenScale;
  return GeoBounds(position, size, getRotation()).pointIsWithinBounds(mousePosition);
}
}