using System.Diagnostics;
using CMEngine;

[Property]
public class MyProperty
{
    public float MyMember;
}

[Property]
public class Prop
{
}

[RequireProperty(typeof(Transform3D))]
public class Example : GameBehaviour
{
    private Transform3D t;
    private Vec3 v;

    public override void onStart()
    {
        t = Entity.GetProperty<Transform3D>();
        t.position = new Vec3(3f, 4f, 2f);
        v = new Vec3(3f, 3f, 3f);

        Logger.Debug(t.position);
        Logger.Debug(v);

        // v = Vec3.Lerp(v, new Vec3(), (float)Time.dt);
    }

    public override void onRun()
    {
        t.position = Vec3.Lerp(t.position, new Vec3(), (float)Time.dt);

        if (t.position <= Vec3.one)
        {
            Logger.Debug("Siamo arrivati.");
            return;
        }
        else Logger.Succeed(t.position);
    }
}