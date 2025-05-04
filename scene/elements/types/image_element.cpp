#include "image_element.h"

//TODO: Decide if we wanna load the image when we need it and release it when we don't
//or... we can just keep it in the memory

ImageElement::ImageElement(const char* id, vec2_t position, GameTexture* texture) : Element(id, position) {
	this->texture = texture;
	this->bound.x = texture->width;
	this->bound.y = texture->height;
}

void ImageElement::Render() {
	Renderer* renderer = Renderer::GetInstance();
	renderer->RenderTexture(this->texture, this->position.x, this->position.y, this->alignment);
}