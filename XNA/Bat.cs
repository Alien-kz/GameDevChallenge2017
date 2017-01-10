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
    class Bat: IDrawable
    {
        public Bat(int bound_x, int bound_y, bool left)
        {
            window_width = bound_x;
            window_height = bound_y;
            is_left = left;
            position = left ? new Vector2(width, 0) : new Vector2(window_width - 2 * width, 0);
            bubble = (int)position.Y + height / 2;
            bubble_speed = 0;
        }
        #region Члены IDrawable

        public void Draw(GameTime gameTime)
        {
            throw new NotImplementedException();
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
        public bool is_left;
        public int window_width;
        public int window_height;
        public int width = 25;
        public int height = 200;
        public Vector2 position = new Vector2(0, 0);
        public int speed = 10;             //pixels per press
        public Keys left_bat_up = Keys.W;           //controls
        public Keys left_bat_down = Keys.S;
        public Keys right_bat_up = Keys.Up;
        public Keys right_bat_down = Keys.Down;
        public int score = 0;
        public int bubble = 0;
        public int bubble_speed = 0;
        //public int bubble_acceleration = 0;
        //public Ball ball;

        public void Update(GameTime gameTime)
        {
            if ((Keyboard.GetState().IsKeyDown(left_bat_up) && is_left) || (Keyboard.GetState().IsKeyDown(right_bat_up) && !is_left))
                position.Y -= speed;
            if ((Keyboard.GetState().IsKeyDown(left_bat_down) && is_left) || (Keyboard.GetState().IsKeyDown(right_bat_down) && !is_left))
                position.Y += speed;
            if (position.Y < 0)
                position.Y = 0;
            if (position.Y + height > window_height)
                position.Y = window_height - height;
            //bubble
            int place_for_bubble = (int)position.Y + height / 2;
            bubble_speed = (place_for_bubble - bubble) / 10;
            bubble += bubble_speed;
        }
       
    }
}
