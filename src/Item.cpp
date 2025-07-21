#include "Item.h"

Item Item::Clone() {
        Item newItem;
        newItem.texture = this->texture;
        newItem.sprite = new sf::Sprite(*newItem.texture);
        newItem.sprite->setScale(this->sprite->getScale());
        newItem.id = this->id;
        newItem.price = this->price;
        newItem.name = this->name;
        newItem.tag = this->tag;
        newItem.priceModifier = this->priceModifier;

        return newItem;
};