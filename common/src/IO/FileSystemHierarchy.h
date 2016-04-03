/*
 Copyright (C) 2010-2016 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_FileSystemHierarchy
#define TrenchBroom_FileSystemHierarchy

#include "SharedPointer.h"
#include "StringUtils.h"
#include "IO/FileSystem.h"
#include "IO/Path.h"

#include <vector>

namespace TrenchBroom {
    namespace IO {
        class Path;
        
        class FileSystemHierarchy : public virtual FileSystem {
        private:
            typedef std::vector<FileSystem*> FileSystemList;
            FileSystemList m_fileSystems;
        public:
            FileSystemHierarchy();
            virtual ~FileSystemHierarchy();
            
            void addFileSystem(FileSystem* fileSystem);
            virtual void clear();
        private:
            Path doMakeAbsolute(const Path& relPath) const;
            bool doDirectoryExists(const Path& path) const;
            bool doFileExists(const Path& path) const;
            FileSystem* findFileSystemContaining(const Path& path) const;
            
            Path::List doGetDirectoryContents(const Path& path) const;
            const MappedFile::Ptr doOpenFile(const Path& path) const;
        private:
            FileSystemHierarchy(const FileSystemHierarchy& other);
            FileSystemHierarchy& operator=(FileSystemHierarchy other);
        };
        
        class WritableFileSystemHierarchy : public FileSystemHierarchy, public WritableFileSystem {
        private:
            WritableFileSystem* m_writableFileSystem;
        public:
            WritableFileSystemHierarchy();
            
            void addReadableFileSystem(FileSystem* fileSystem);
            void addWritableFileSystem(WritableFileSystem* fileSystem);
            void clear();
        private:
            void doCreateDirectory(const Path& path);
            void doDeleteFile(const Path& path);
            void doCopyFile(const Path& sourcePath, const Path& destPath, bool overwrite);
            void doMoveFile(const Path& sourcePath, const Path& destPath, bool overwrite);
        private:
            WritableFileSystemHierarchy(const WritableFileSystemHierarchy& other);
            WritableFileSystemHierarchy& operator=(WritableFileSystemHierarchy other);
        };
    }
}

#endif /* defined(TrenchBroom_FileSystemHierarchy) */
