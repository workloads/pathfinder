#pragma once

struct vector
{
    float r;
    float l;
};


String toString(vector* self){
    return "L: " + String(self->l) +" R: " + String(self->r);
}

struct step
{
    int angle;
    String direction;
    int distance;
};
