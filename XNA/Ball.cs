using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace NewPong
{
    class Ball: IDrawable
    {
        #region Члены IDrawable

        public void Draw(GameTime gameTime)
        {
        }

        public int DrawOrder
        {
            get { throw new NotImplementedException(); }
        }

        public event EventHandler DrawOrderChanged;

        public bool Visible
        {
            get { throw new NotImplementedException(); }
        }

        public event EventHandler VisibleChanged;

        #endregion

        public Ball(int bound_x, int bound_y)
        {
            window_width = bound_x;
            window_height = bound_y;
            position = new Vector2((window_width - width) / 2, (window_height - height) / 2);
            speed = new Vector2(0.04f, 0.04f);            //pixels per ms
            acceleration = new Vector2(0.0001f, 0.0001f);   //pixels per ms^2
            maxspeed = 1;
        }
        public static int window_width;
        public static int window_height;
        public static int width = 25;
        public static int height = 25;
        public Vector2 position;
        public Vector2 speed;
        public Vector2 acceleration;
        public float maxspeed;
        public Bat left_bat, right_bat;

        public void Update(GameTime gameTime)
        {
            position += speed * gameTime.ElapsedGameTime.Milliseconds;
            if (speed.Length() < maxspeed)
                speed += acceleration * gameTime.ElapsedGameTime.Milliseconds;
            Check();
        }

        public void Check()
        {
            
            if (position.X < 0)
            {
                position = new Vector2((window_width - width) / 2, (window_height - height) / 2);
                speed = new Vector2(0.002f, 0.002f);            //pixels per ms
                acceleration = new Vector2(0.00003f, 0.00004f);   //pixels per ms^2
                right_bat.score++;
            }
            if (position.X + width > window_width)
            {
                position = new Vector2((window_width - width) / 2, (window_height - height) / 2);
                speed = new Vector2(0.002f, 0.002f);            //pixels per ms
                acceleration = new Vector2(0.00003f, 0.00004f);   //pixels per ms^2
                left_bat.score++;
            }
            if (position.Y < 0)
            {
                position.Y *= -1;
                speed.Y *= -1;
                acceleration.Y *= -1;
            }
            if (position.Y + height > window_height)
            {
                position.Y = 2 * (window_height - height) - position.Y;
                speed.Y *= -1;
                acceleration.Y *= -1;
            }
            if (position.X + width >= right_bat.position.X && position.Y + width >= right_bat.position.Y && position.Y <= right_bat.position.Y + right_bat.height)
            {
                //simple ricochet
                /*
                position.X = 2 * (right_bat.position.X - width) - position.X;
                speed.X *= -1;
                acceleration.X *= -1;
                 */
                float fwidth = (float)width;
                float late_for = (position.X + fwidth - right_bat.position.X) / speed.X;
                float hit_point_Y = position.Y + fwidth / 2 - speed.Y * late_for;
                float min_Y = right_bat.position.Y - fwidth / 2;
                float max_Y = right_bat.position.Y + right_bat.height + fwidth / 2;
                float coeff = (hit_point_Y - min_Y) / (max_Y - min_Y);
                float velocity = speed.Length();
                if (speed.Y >= 0)
                    speed.Y = coeff * (0.9f * velocity + speed.Y) - speed.Y;
                else
                    speed.Y = -coeff * (-0.9f * velocity + speed.Y) - 0.9f * velocity;
                speed.X = -(float)Math.Sqrt(velocity * velocity - speed.Y * speed.Y);
                acceleration = speed * acceleration.Length() / speed.Length();

            }
            if (position.X <= left_bat.position.X + left_bat.width && position.Y + width >= left_bat.position.Y && position.Y <= left_bat.position.Y + left_bat.height)
            {
                /*
                position.X = 2 * (left_bat.position.X + left_bat.width) - position.X;
                speed.X *= -1;
                acceleration.X *= -1;
                 */
                float fwidth = (float)width;
                float late_for = (position.X - left_bat.position.X - left_bat.width) / speed.X;
                float hit_point_Y = position.Y + fwidth / 2 - speed.Y * late_for;
                float min_Y = left_bat.position.Y - fwidth / 2;
                float max_Y = left_bat.position.Y + left_bat.height + fwidth / 2;
                float coeff = (hit_point_Y - min_Y) / (max_Y - min_Y);
                float velocity = speed.Length();
                if (speed.Y >= 0)
                    speed.Y = coeff * (0.9f * velocity + speed.Y) - speed.Y;
                else
                    speed.Y = -coeff * (-0.9f * velocity + speed.Y) - 0.9f * velocity;
                speed.X = (float)Math.Sqrt(velocity * velocity - speed.Y * speed.Y);
                acceleration = speed * acceleration.Length() / speed.Length();
            }
        }
    }
}
