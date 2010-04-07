using System;
using System.Collections.Generic;
using System.Web;

namespace BuildsVisualization
{
    public class CommitInfo
    {
        protected int revision;

        public int Revision
        {
            get
            {
                return revision;
            }
        }


        protected string message;

        public string Message
        {
            get
            {
                return message;
            }
        }

        protected string autor;

        public string Autor
        {
            get
            {
                return autor;
            }
        }

        public CommitInfo(int revision, string message, string autor)
        {
            this.revision = revision;
            this.message = message;
            this.autor = autor;
        }

        public CommitInfo()
        {

        }
    }
}
