#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    RoutePlanner::start_node = &m_Model.FindClosestNode(start_x, start_y);
    RoutePlanner::end_node = &m_Model.FindClosestNode(end_x, end_y);

}

// TODO 3: Implement the CalculateHValue method.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    //Use the distance method on node object to find the distance to the end_node for the h value.
    float h_value =  node->distance(*RoutePlanner::end_node);
    return h_value;
}

// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    //Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
    current_node->FindNeighbors();
    if(current_node->neighbors.size()>0){
        for(RouteModel::Node *neighbor : current_node->neighbors){
            //For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
            neighbor->parent = current_node;
            neighbor->h_value = RoutePlanner::CalculateHValue(neighbor);
            neighbor->g_value = current_node->g_value + neighbor->distance(*current_node);
            //For each node in current_node.neighbors, add the neighbor to open_list
            RoutePlanner::open_list.push_back(neighbor);
            //set the node's visited attribute to true.
            neighbor->visited = true;
        }
    }
}

// TODO 5: Complete the NextNode method to sort the open list and return the next node.

// Compare sub function which is used in NextNode member function
bool fCompare(RouteModel::Node *a, RouteModel::Node *b){
    float f1 = a->h_value + a-> g_value;
    float f2 = b->h_value + b-> g_value;
    return(f1<f2);
}

RouteModel::Node *RoutePlanner::NextNode() {
    // Sort the open_list according to the sum of the h value and g value.
    std::sort(RoutePlanner::open_list.begin(), RoutePlanner::open_list.end(), fCompare);
    // Create a pointer to the node in the list with the lowest sum.
    RouteModel::Node *lowest_node = RoutePlanner::open_list.front();
    // Remove that node from the open_list.
    RoutePlanner::open_list.erase(RoutePlanner::open_list.begin());
    // Return the pointer.
    return lowest_node;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    RouteModel::Node *parent_node = nullptr; //Initiate the path_found with a nullptr.
    path_found.push_back(*current_node);

    // This method should take the current (final) node as an argument and iteratively follow the 
    // chain of parents of nodes until the starting node is found.
    while(current_node != RoutePlanner::start_node){
        parent_node = current_node-> parent;
        path_found.push_back(*parent_node);
        // For each node in the chain, add the distance from the node to its parent to the distance variable.
        distance += current_node->distance(*parent_node);
        current_node = parent_node;
    }

    // The returned vector should be in the correct order: 
    // The start node should be the first element of the vector, the end node should be the last element.
    // Reverse the path_found to bring it to correct order
    std::reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
    current_node = RoutePlanner::start_node;
    current_node->visited = true;
    RoutePlanner::open_list.push_back(current_node);
    while(RoutePlanner::open_list.size() > 0){
        // Use the NextNode() method to sort the open_list and return the next node.
        current_node = RoutePlanner::NextNode();
        if(current_node != RoutePlanner::end_node){
            // Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
            RoutePlanner::AddNeighbors(current_node);
        }
        else{
            break;
        }
    }
    // When the search has reached the end_node, 
    // use the ConstructFinalPath method to return the final path that was found
    // Store the final path in the m_Model.path attribute before the method exits. 
    // This path will then be displayed on the map tile.
    m_Model.path = RoutePlanner::ConstructFinalPath(current_node);
}