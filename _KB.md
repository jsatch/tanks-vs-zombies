- There was a problem when the game starts
and the pawn already has a camera attached. 
The problem was that the camera was pointing
to anywhere.

The fix was to configure camera AutoPosses 
property to Player0.

- You should restart the editor after making 
changes to a object constructor. If you dont
do that, editor will have an strange behaviour.

El uso de *BlueprintNativeEvent* se debe a que se define
un método que deberá de implementarse dentro de un blueprint,
pero en caso que no se haga, se ejecutará la implementación 
por defecto de c++. https://answers.unrealengine.com/questions/434821/how-blueprintnativeevent-specifier-works.html


