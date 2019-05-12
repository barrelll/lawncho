import os, glob

class conf:
    """
    Class config, used for finding and parsing c++ files,
    and routing to correct sources
    """
    @staticmethod
    def search_headDir( headDirectory ):
        """
        Returns a list of filePaths, starting from 'headDirectory'
        """
        filePath = []
        for dirpath, _, _ in os.walk( headDirectory ):
            filePath += glob.glob( dirpath + '/*.h' )
        return filePath
