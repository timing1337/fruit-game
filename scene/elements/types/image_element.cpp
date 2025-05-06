#include "image_element.h"
ImageElement::ImageElement(const char* id, vec2_t position, GameTexture* texture) : Element(id, position) {
	this->texture = texture;
	this->bound.x = texture->width;
	this->bound.y = texture->height;
}

void ImageElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y, this->alignment, this->alignmentVertical);
}