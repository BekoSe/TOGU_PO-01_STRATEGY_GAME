#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H

#include "windows.h"
#include "Animation.h"
#include "GameObjectTypes.h"


class cScene;
class cMap;
class cParam;

class cGameObject
{   
protected:
    //Статические указатели на иные объекты, используемые всеми классами, наследующими от cGameObject
    static cScene* scene; //Указатель на сцену
    static cMap* map; //Указатель на карту
    static cParam* params; //Указатель на файл с параметрами

    SelfAnimation anim; //Структура анимации объекта

    //Геометрические характеристики объекта:
    float pos[3]; //Положение объекта на сцене
    float width; //Ширина спрайта объекта
    float height; //Высота спрайта объекта
    float rad; //Радиус, занимаемый объектом на карте


    //Переменные состояния
    int type_id; //Идентификатор типа объекта
    bool attackable; //Флаг, показывающий, можно ли объект атаковать
    bool selectable; //Флаг, показывающий можно ли выделить объект
    bool selected; //Флаг, показывающий, выделен ли объект в текущий момент времени
    bool visible; //Флаг видимости объекта
    int cur_state; //Текущее состояние объекта (строится, стоит, идёт, добывает ресурсы и т.д.)
    float max_life; //Максимальное количество жизни объекта (для ресурсов - объём ресурса)
    float cur_life; //Текущее количество жизни (для ресурсов - текущий объем ресурса)

    //Технические поля
    bool del_flag; //Флаг готовности объекта к удалению
    int obj_id; //Индекс объекто в структуре сцены (для поддержки различных механизмов хранения объектов)
public:    
    cGameObject();
    static void SetPointers(cMap* m, cScene* s, cParam* p); //Статический метод инициализации указателей на необходимые объекты
    virtual void Select(bool multiple); //Выделение объекта (multiple=true - множественное)
    virtual void Unselect(); //Снятие выделения
    virtual void Action(float* click_pos, cGameObject* target); //Дествие, совершаемое с выделенными объектами
    virtual void Calculate(); //Вычисление состояний объекта через заданный интервал времени
    virtual void Draw(); //Рисование объекта
    void Pos(float* p); //Прочитать положение объекта
    float Rad(); //Узнать радиус объекта
    bool IsSelectable(); //Узнать, может ли объект быть выделен
    bool IsSelected(); //Узнать, выделен ли объект
    bool IsAttackable(); //Узнать, можно ли объект атаковать
    virtual float Attack(float power); //Атаковать объект, возвращает количество снятых жизней
    bool ReadyToDel(); //Узнать, готов ли объект к удалению
    void SetIndex(int id);//Установка индекса объекта в глобальной структуре cScene
    int  GetIndex();//Возвращает индекс объекта в глобальной структуре cScene
    virtual ~cGameObject();
};

#endif // CGAMEOBJECT_H
