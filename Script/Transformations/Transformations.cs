using System;
using System.Runtime.InteropServices;

namespace CMEngine.Script.Transformations
{
    public class Transform3D : IDisposable
    {
        private IntPtr _instance;
        private bool _disposed = false; // To detect redundant calls

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CreateTransform3D();

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DestroyTransform3D(IntPtr instance);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetPosition(IntPtr instance, float x, float y, float z);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPosition(IntPtr instance, out float x, out float y, out float z);

        public Transform3D()
        {
            _instance = CreateTransform3D();
        }

        // Implement IDisposable
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        // Protected implementation of Dispose pattern.
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_instance != IntPtr.Zero)
                {
                    DestroyTransform3D(_instance);
                    _instance = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        // Destructor
        ~Transform3D()
        {
            Dispose(false);
        }
        
        public void SetPosition(float x, float y, float z)
        {
            SetPosition(_instance, x, y, z);
        }

        public void SetPosition(Vec3 pos)
        {
            SetPosition(_instance, pos.x, pos.y, pos.z);
        }

        public Vec3 position
        {
            get
            {
                if (_instance == IntPtr.Zero)
                {
                    return new Vec3();
                }
                GetPosition(_instance, out float x, out float y, out float z);
                return new Vec3(x, y, z);
            }
            set
            {
                SetPosition(value);
            }
        }
        
        //     public float X
//     {
//         get
//         {
//             GetPosition(_instance, out float x, out _, out _);
//             return x;
//         }
//         set
//         {
//             GetPosition(_instance, out _, out float y, out float z);
//             SetPosition(_instance, value, y, z);
//         }
//     }
//
//     public float Y
//     {
//         get
//         {
//             GetPosition(_instance, out _, out float y, out _);
//             return y;
//         }
//         set
//         {
//             GetPosition(_instance, out float x, out _, out float z);
//             SetPosition(_instance, x, value, z);
//         }
//     }
//
//     public float Z
//     {
//         get
//         {
//             GetPosition(_instance, out _, out _, out float z);
//             return z;
//         }
//         set
//         {
//             GetPosition(_instance, out float x, out float y, out _);
//             SetPosition(_instance, x, y, value);
//         }
//     }
    }
}