using System;
using System.Runtime.InteropServices;

namespace CMEngine.Math
{
    public class Vec3 : IDisposable
    {
        private IntPtr _instance;
        private bool _disposed = false; // To detect redundant calls

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CreateVec3(float x, float y, float z);
    
        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DestroyVec3(IntPtr instance);
    
        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Lerp(IntPtr v1, IntPtr v2, float t);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Cross(IntPtr v1, IntPtr v2);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Normalize(IntPtr instance);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern double getX(IntPtr instance);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern double getY(IntPtr instance);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern double getZ(IntPtr instance);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void setX(IntPtr instance, float x);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void setY(IntPtr instance, float y);

        [DllImport("CMEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void setZ(IntPtr instance, float z);
        
        public float x
        {
            get
            {
                CheckIfDisposed();
                return (float)getX(_instance);
            }
            private set
            {
                CheckIfDisposed();
                setX(_instance, value);
            }
        }
        public float y
        {
            get {
                CheckIfDisposed();
                return (float)getY(_instance);
            }
            private set
            {
                CheckIfDisposed();
                setY(_instance, value);

            }
        }
        public float z
        {
            get
            {
                CheckIfDisposed();
                return (float)getZ(_instance);
            }
            private set
            {
                CheckIfDisposed();
                setZ(_instance, value);
            }
        }
        
        public Vec3()
        {
            _instance = CreateVec3(0, 0, 0);
            Initialize(0, 0, 0);
        }

        public Vec3(float x, float y, float z)
        {
            _instance = CreateVec3(x, y, z);
            Initialize(x, y, z);
        }

        public Vec3(IntPtr instance)
        {
            _instance = instance;
        }

        public static Vec3 zero
        {
            get { return new Vec3(0, 0, 0); }
        }
        
        public static Vec3 one
        {
            get { return new Vec3(1f, 1f, 1f); }
        }
        
        private void Initialize(float x, float y, float z)
        {
            CheckIfDisposed();
            setX(_instance, x);
            setY(_instance, y);
            setZ(_instance, z);
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
                    DestroyVec3(_instance);
                    _instance = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        // Destructor
        ~Vec3()
        {
            Dispose(false);
        }

        public static Vec3 Lerp(Vec3 v1, Vec3 v2, float t)
        {
            IntPtr resultPtr = Lerp(v1._instance, v2._instance, t);
            if (resultPtr == IntPtr.Zero) throw new Exception("Failed to compute Lerp");
            Vec3 result = new Vec3(resultPtr);
            return result;
        }

        public static Vec3 Cross(Vec3 v1, Vec3 v2)
        {
            IntPtr result = Cross(v1._instance, v2._instance);
            return new Vec3(result);
        }

        public Vec3 Normalize()
        {
            IntPtr result = Normalize(_instance);
            return new Vec3(result);
        }

        public static Vec3 operator +(Vec3 v, float s)
        {
            return new Vec3(v.x + s, v.y + s, v.z + s);
        }
        
        public static Vec3 operator +(Vec3 v, Vec3 w)
        {
            return new Vec3(v.x + w.x, v.y + w.y, v.z + w.z);
        }

        public static bool operator <(Vec3 v, Vec3 w)
        {
            return (v.x < w.x && v.y < w.y && v.z < w.z);
        }
        
        public static bool operator >(Vec3 v, Vec3 w)
        {
            return (v.x > w.x && v.y > w.y && v.z > w.z);
        }
        
        public static bool operator <=(Vec3 v, Vec3 w)
        {
            return (v.x <= w.x && v.y <= w.y && v.z <= w.z);
        }
        
        public static bool operator >=(Vec3 v, Vec3 w)
        {
            return (v.x >= w.x && v.y >= w.y && v.z >= w.z);
        }
        
        public static bool operator ==(Vec3 v, Vec3 w)
        {
            return (v.x == w.x && v.y == w.y && v.z == w.z);
        }
        
        public static bool operator !=(Vec3 v, Vec3 w)
        {
            return (v.x != w.x || v.y != w.y || v.z != w.z);
        }
        
        public override string ToString()
        {
            return $"Vec3({x}, {y}, {z})";;
        }

        // Internal method to handle disposal
        internal void DisposeInternal()
        {
            Dispose();
        }
        
        private void CheckIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException("Vec3");
        }
    }
}