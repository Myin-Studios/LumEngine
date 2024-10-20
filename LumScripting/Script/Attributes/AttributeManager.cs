﻿using System;

namespace LumScripting.Script.Attributes
{
    [AttributeUsage(AttributeTargets.Class)]
    public class PropertyAttribute : Attribute
    {
        // TODO
    }

    [AttributeUsage(AttributeTargets.Class, Inherited = false, AllowMultiple = true)]
    public class RequirePropertyAttribute : Attribute
    {
        public Type PropertyType { get; }

        public RequirePropertyAttribute(Type propertyType)
        {
            PropertyType = propertyType;
        }
    }

}

